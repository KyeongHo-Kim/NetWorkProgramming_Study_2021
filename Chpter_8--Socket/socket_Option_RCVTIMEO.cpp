/*
    default로 timeout을 설정하지 않으면 blocking mod로 무한하게 기다린다.
    socket option 의 timeout에 의해 클라이언트가 종료되면 socket_error 를 반환한다. 즉 정상적으로 종료되지 않은것이다.
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

	int rcvtime = 5000;
	int  rcvtimelen = sizeof(rcvtime);
	if (setsockopt(c_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&rcvtime, rcvtimelen) == SOCKET_ERROR)
	{
		Err_Display("setsockopt_RCVTIMEOUT");
		return -1;
	}

	char buf[80];
	int recvlen;
	while (true)
	{
		recvlen = recv(c_sock, buf, 80, 0); 
		if (recvlen == 0)
		{
			cout << "client connetion Nomal close case" << endl;
			closesocket(c_sock);
			break;
		}
		if (recvlen == SOCKET_ERROR)
		{
			cout << "client connetion abNomal close case"<<endl;
			Err_Display("recv()");
			closesocket(c_sock);
			break;
		}

		if (recvlen >= 0 && recvlen < 80)
		{
			buf[recvlen] = NULL;
		}

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