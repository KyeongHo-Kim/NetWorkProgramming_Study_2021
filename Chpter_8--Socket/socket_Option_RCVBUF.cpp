/*
    RCVBUF의 default size는 windows10 기준 약 64 정도로 출력된다.
    recv socket buf를 0으로 셋팅하고 메시지를 보냈는데 정상작동 하는것을 확인했다.

    클라이언트에서 data를 보내면 socket buf가 수신받게 된다. socket은 송·수신 2개의 buf를 가지고 있다.
    이때 "수신"버퍼의 크기를 0으로 설정하면         << (setsockopt - SO_RCVBUF 사용)
    tcp_buf(Windows Size)에서 data를 받고, socket buf로 pumping한다.
    socket buf는 받은 data를 recv buf로 pumping 한다.

    즉 최초로 data를 받는 공간은 socket buf이고, 이때 socket buf size가 0이면 tcp buf(windows size)에서 받은 다음 socket buf를 거쳐 recv buf로 전달해준다.
*/
#pragma comment(lib,"ws2_32.lib")
#include<winsock2.h>
#include<WS2tcpip.h>
#include<iostream>

using std::endl;
using std::cout;

void Err_Display(const char *);
void Print_IP(LPVOID);
void Insert_IP(PCSTR, PVOID);

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		Err_Display("WSAStartup()");
		return -1;
	}

	SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (tcpSocket == INVALID_SOCKET)
	{
		Err_Display("tcpSocket");
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	//Insert_IP("127.0.0.1",&saddr.sin_addr);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(tcpSocket, (SOCKADDR *)&saddr, sizeof(saddr)))
	{
		Err_Display("bind()");
		return -1;
	}

	if (listen(tcpSocket, SOMAXCONN))
	{
		Err_Display("listen()");
		return -1;
	}

	SOCKADDR_IN caddr;
	ZeroMemory(&caddr, sizeof(caddr));
	int caddrlen = sizeof(caddr);

	cout << "Waiting for connet .... ";
	SOCKET c_sock = accept(tcpSocket, (SOCKADDR *)&caddr, &caddrlen);
	if (c_sock == INVALID_SOCKET)
	{
		Err_Display("accept()");
		return -1;
	}
	cout << "Connected !!!" << endl;

	int rcvbuf = 0;
	int  rcvbuflen = sizeof(rcvbuf);
	/*if (setsockopt(c_sock, SOL_SOCKET, SO_RCVBUF, (char *)&rcvbuf, rcvbuflen) == SOCKET_ERROR)
	{
		Err_Display("setsockopt_RCVBUF");
		return -1;
	}*/

	if (getsockopt(c_sock, SOL_SOCKET, SO_RCVBUF, (char *)&rcvbuf, &rcvbuflen) == SOCKET_ERROR)
	{
		Err_Display("getsockopt_RCVBUF");
		return -1;
	}
	cout << "Recv_Buf_Size : " << rcvbuf/1024 << endl;

	char buf[80];
	int recvlen;
	while (true)
	{
		recvlen = recv(c_sock, buf, 80, 0);
		if (recvlen == 0 || recvlen == SOCKET_ERROR)
		{
			cout << "client connetion close case";
			closesocket(c_sock);
			break;
		}

		buf[recvlen] = NULL;
		cout << "From Client: " << buf << endl;

		send(c_sock, buf, recvlen, 0);
	}

	return 0;
}

void Err_Display(const char *mes)
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
	cout << mes << " : " << (LPSTR)err_mes << endl;
	LocalFree(err_mes);
}

void Print_IP(LPVOID addr)
{
	char buf[20];
	if (!InetNtop(AF_INET, addr, buf, 20))
	{
		Err_Display("InetNtop()");
		return;
	}

	cout << "IP Address: " << buf << endl;
}

void Insert_IP(PCSTR str, PVOID addr)
{
	if (!InetPton(AF_INET, str, addr))
	{
		Err_Display("InetPton()");
		return;
	}
}