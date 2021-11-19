/*
	-selset-

	1. fd_set 메모리 생성 즉 소켓의 행동을 감시할 메모리를 만듬
	2. 해당 메모리 초기화 FD_ZERO(&memory)사용
	3. 소켓과 fd_set 변수를 묶음 FD_SET(socket, &memory) 해당 메모리가 socket을 감시
	4. select함수가 반환하는 값 즉 소켓 갯수를 저장할 int 변수와 원본 데이터 유지를 위한 fd_set copy변수를 만든다.
	- loop -
	5. selsect()함수 작성 socket_error의 경우 format message 호출
	6. select함수에서 반환된 소켓을 확인하기위해 for반복을 돌린다.
	7. 1차 copy.array의 인덱스가 copy 메모리에 있다면 fd_isset 함수 사용
	8. 2차 copy.array의 인덱스가 g_sock과 같다면 aceept를 하여 새로운 소켓을 만든다.
	9. 새로만들어진 소켓을 reads 메모리에 추가한다.

	10. 8번 즉 g_sock이 아니면 recv함수 send함수 작성 여기서 error작성시 fd_clr함수를 사용한다.
*/
#pragma comment(lib,"ws2_32.lib")
#include<iostream>
#include<WinSock2.h>
#include<process.h>
#include<WS2tcpip.h>

using std::cout;
using std::endl;

void Err_Display(const char*);
void Insert_IP(PCSTR, PVOID);

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2),&wsa))
	{
		Err_Display("WSAStartup()");
		return -1;
	}

	SOCKET g_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_sock == INVALID_SOCKET)
	{
		Err_Display("g_sock");
		return -1;
	}

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bool reus = true;
	if (setsockopt(g_sock,SOL_SOCKET,SO_REUSEADDR,(char*)&reus,sizeof(reus) == SOCKET_ERROR))
	{
		Err_Display("setsock(REUSADDR)");
		return -1;
	}

	if (bind(g_sock,(SOCKADDR*)&saddr,sizeof(saddr)))
	{
		Err_Display("bind()");
		return -1;
	}
	
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(g_sock, &reads);

	if (listen(g_sock,SOMAXCONN))
	{
		Err_Display("listen()");
		return -1;
	}

	int ret_select;
	fd_set copy_reads;
	SOCKET tcp_sock;
	SOCKADDR_IN caddr;
	int caddrlen = sizeof(caddr);
	int recvlen;
	char buf[80];
	while (true)
	{
		copy_reads = reads;
		ret_select = select(0, &copy_reads, NULL, NULL, NULL);
		if (ret_select == SOCKET_ERROR)
		{
			Err_Display("select");
			break;
		}

		for (int idx = 0; idx < ret_select; idx++)
		{
			if (FD_ISSET(copy_reads.fd_array[idx], &copy_reads))
			{
				if (copy_reads.fd_array[idx] == g_sock)
				{
					tcp_sock = accept(g_sock, (SOCKADDR*)&caddr, &caddrlen);
					if (tcp_sock == INVALID_SOCKET)
					{
						Err_Display("accept");
						return -1;
					}
					FD_SET(tcp_sock, &reads);
				}
				else
				{
					recvlen = recv(copy_reads.fd_array[idx], buf, 80, 0);
					if (recvlen == 0)
					{
						cout << "Nomal close case" << endl;
						closesocket(copy_reads.fd_array[idx]);
						FD_CLR(copy_reads.fd_array[idx], &reads);
						continue;
					}
					if (recvlen == SOCKET_ERROR)
					{
						cout << "abNomal close case" << endl;
						Err_Display("recv()");
						closesocket(copy_reads.fd_array[idx]);
						FD_CLR(copy_reads.fd_array[idx], &reads);
						continue;
					}
					buf[recvlen] = NULL;
					cout << "From Client: " << buf << endl;

					send(copy_reads.fd_array[idx], buf, recvlen, 0);
				}
			}
		}
	}

	closesocket(g_sock);
	WSACleanup();
	return 0;
}

void Err_Display(const char* mes)
{
	LPVOID err_mes = NULL;

	FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&err_mes,
		0,
		NULL
	);
	cout << (LPSTR)err_mes << endl;
	LocalFree(err_mes);
}

void Insert_IP(PCSTR str, PVOID addr)
{
	if (!InetPton(AF_INET,str,addr))
	{
		Err_Display("InetPton");
		return;
	}
}