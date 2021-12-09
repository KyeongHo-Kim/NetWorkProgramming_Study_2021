#include "Over1.hpp";

#define lock(a) EnterCriticalSection(&a);
#define unlock(a) LeaveCriticalSection(&a);

bool Server_Init(Server* ser)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2),&wsa))
	{
		Err_Display("WSAStartup");
		return false;
	}

	SOCKET g_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_sock == INVALID_SOCKET)
	{
		Err_Display("g_sock");
		return false;
	}

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(g_sock,(SOCKADDR*)&saddr,sizeof(saddr)))
	{
		Err_Display("bind()");
		return false;
	}

	// 서버의 정보를 초기화한다.
	ser->gate_sock = g_sock;
	ser->noc = 0;
	InitializeCriticalSection(&ser->cs);

	return true;
}

Session* Add_Session(Server* ser, SOCKET sock)	// accept에 의해 만들어진 소켓이 넘어옴	
{
	// 1. 새로운 세션 메모리를 생성한다.
	Session* ses = new Session;		

	// 2. 세션의 정보를 초기화한다.

	// 버퍼를 연결한다.
	ses->rwsa.buf = ses->rbuf;
	ses->swsa.buf = ses->sbuf;

	// Session::tcp_sock에 accept sock을 복사
	ses->tcp_sock = sock;
	ses->event = WSACreateEvent();

	// 통로 역할 서버로 가기위함
	ses->server = ser;

	// 3. 서버 정보 업데이트
	lock(ser->cs);						// 잠금

	ser->sarr.push_back(ses);			// 1 ~ 2번 과정에서 만든 세션 포인터를 서버에 추가
	ser->earr.push_back(ses->event);	// 세션의 이벤트객체를 서버에 추가
	ser->noc++;							

	unlock(ser->cs);				    // 잠금 해제

	return ses;
}

void Close_Session(Session* ses)
{
	SOCKET s = ses->tcp_sock;
	WSAEVENT e = ses->event;
	 
	Server* ser = ses->server;
	lock(ser->cs);				// 잠금

	ser->sarr.erase(find(ser->sarr.begin(), ser->sarr.end(), ses));
	ser->earr.erase(find(ser->earr.begin(), ser->earr.end(), e));
	ser->noc--;

	unlock(ser->cs);		   // 잠금 해제

	closesocket(s);
	WSACloseEvent(e);
}

void Clean_Session(Session* ses, int size, IOtype type)
{
	//WSAOVERLAPPED 멤버 초기화
	ZeroMemory(&ses->over, sizeof(ses->over));
	ses->over.hEvent = ses->event;

	if (type == IOtype::Read)
	{
		ses->rwsa.len = size - 1;
		ses->handle = Read_Process;
	}
	else
	{
		ses->swsa.len = size;
		ses->handle = Write_Process;
	}
}


void Packet_Recv(Session* ses)
{
	Clean_Session(ses, 80, IOtype::Read);

	DWORD trans;
	DWORD flag = 0;

	int recv_result = WSARecv(ses->tcp_sock, &ses->rwsa, 1, &trans, &flag, &ses->over, NULL);
	if (recv_result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		Err_Display("WSARecv()");
	}
}

void Packet_Send(Session* ses, int size)
{
	Clean_Session(ses, size, IOtype::Write);

	DWORD trans;
	int send_result = WSASend(ses->tcp_sock, &ses->swsa, 1, &trans, 0, &ses->over, NULL);
	if (send_result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		Err_Display("WSASend()");
	}
}

void Read_Process(Session* ses, DWORD trans)
{
	if (trans == 0)
	{
		cout << "nomal close case" << endl;
		Close_Session(ses);
		return;
	}

	// 수신 데이터 출력
	ses->rbuf[trans] = NULL;
	cout << "From Client: " << ses->rbuf << endl;

	// 수신 데이터 Echo Service 하기위해 sbuf에 복사
	CopyMemory(ses->sbuf, ses->rbuf, trans);
	Packet_Send(ses, trans);
}

// 이벤트 객체가 signal상태 즉overlapped io가 끝낫을 경우
void Write_Process(Session* ses, DWORD trans)
{
	Packet_Recv(ses);
}

void Err_Display(const char* mes)
{
	LPVOID err_mes = NULL;
	FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPSTR)&err_mes,
		0,
		NULL
	);
	cout << mes << " : " << (LPSTR)err_mes << endl;
	LocalFree(err_mes);
}