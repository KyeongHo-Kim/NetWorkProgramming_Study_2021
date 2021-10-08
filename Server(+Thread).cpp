/*
1. winsock을 초기화한다.
	- WSAStartup(MAKEWORD(),&)

2. Gate_socket을 생성한다.
	- socket(af,type,protocol)

3. ip와 포트번호를 할당한다.
	- SOCKADDR_IN

4. localaddress와 socket을 열결짓는다.
	- bind(SOCKET,(SOCKARRD*)&,sizeof())

5. server의 상태를 연결가능한 상태로 만든다 listen buf생성
	-listen(SOCKET, SOMAXCONN)

6. 클라이언트와 통신할 전용 소켓을 생성하고 서비스를 승인한다.
	- accept(SOCKET,(SOCKADDR*)&, &size)

7. Thread를 생성한다
	- _beginthreadex(NULL,0,method,arglist,0,NULL)

7.5 recv()와 send()를 함수로 메소드로 만든다.

>	

8. 받은 data를 되돌려준다
	- send(SOCKET,buf,size,0)
*/

#pragma comment(lib,"ws2_32.lib")
#include<winsock2.h>
#include<WS2tcpip.h>
#include<iostream>
#include<process.h>
using std::endl;
using std::cout;

void Err_Display(const char *);
void Print_IP(LPVOID);
void Insert_IP(PCSTR, PVOID);
unsigned __stdcall Echo_Server(LPVOID);

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
	//Insert_IP("127.0.0.1", &saddr.sin_addr);
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

	HANDLE th;

	while (true)
	{
		cout << "Waiting for connet .... ";
		SOCKET c_sock = accept(tcpSocket, (SOCKADDR *)&caddr, &caddrlen);
		if (c_sock == INVALID_SOCKET)
		{
			Err_Display("accept()");
			return -1;
		}
		cout << "Conneted !!!"<<endl;
		th = (HANDLE)_beginthreadex(NULL, 0, &Echo_Server, (LPVOID)c_sock, 0, NULL);
	}

	closesocket(tcpSocket);
	WSACleanup();
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
	cout << "ip addres:" << buf << endl;
}

void Insert_IP(PCSTR str, PVOID addr)
{
	if (!InetPton(AF_INET, str, addr))
	{
		Err_Display("InetPton()");
		return;
	}
}

unsigned __stdcall Echo_Server(LPVOID arg)
{
	SOCKET c_sock = (SOCKET)arg;
	char buf[80];
	int recvlen;

	SOCKADDR_IN caddr;
	int caddrlen = sizeof(caddr);
	getpeername(c_sock, (SOCKADDR*)&caddr, &caddrlen);
	cout << "Server Conneted by: ";
	Print_IP(&caddr.sin_addr);
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