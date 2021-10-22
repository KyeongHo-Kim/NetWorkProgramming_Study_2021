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

7. 수신을 받고 출력한다.
    - recv(SOCKET,buf,size,option)
    -buf[] = NULL

8. 받은 data를 되돌려준다 
    - send(SOCKET,buf,size,0)
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
	if (WSAStartup(MAKEWORD(2, 2),&wsa))
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

	if (bind(tcpSocket,(SOCKADDR*)&saddr,sizeof(saddr)))
	{
		Err_Display("bind()");
		return -1;
	}

	if (listen(tcpSocket,SOMAXCONN))
	{
		Err_Display("listen()");
		return -1;
	}

	SOCKADDR_IN caddr;
	ZeroMemory(&caddr, sizeof(caddr));
	int caddrlen = sizeof(caddr);

	while (true)
	{
		cout << "Waiting for connet .... ";
		SOCKET c_sock = accept(tcpSocket, (SOCKADDR *)&caddr, &caddrlen);
		if (c_sock == INVALID_SOCKET)
		{
			Err_Display("accept()");
			return -1;
		}

		cout<<"Connected !!!"<<endl;
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
	if (!InetNtop(AF_INET,addr,buf,20))
	{
		Err_Display("InetNtop()");
		return;
	}

	cout<<"IP Address: "<<buf<<endl;
}

void Insert_IP(PCSTR str, PVOID addr)
{
	if (!InetPton(AF_INET,str,addr))
	{
		Err_Display("InetPton()");
		return;
	}
}