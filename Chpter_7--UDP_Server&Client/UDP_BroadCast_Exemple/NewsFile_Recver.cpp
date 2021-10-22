#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

using std::cout;
using std::endl;

void Err_Display(const char *);

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		Err_Display("WSAStartup");
		return -1;
	}

	SOCKET g_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (g_sock == INVALID_SOCKET)
	{
		Err_Display("g_sock");
		WSACleanup();
		return -1;
	}


	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(g_sock,(SOCKADDR*)&saddr,sizeof(saddr)))
	{
		Err_Display("bind()");
		return -1;
	}


	char buf[80];
	int recvlen;
	SOCKADDR_IN caddr;
	int caddrlen = sizeof(caddr);
	while (true)
	{
		recvlen = recvfrom(g_sock, buf, 80, 0, (SOCKADDR *)&caddr, &caddrlen);
		if (recvlen == 0)
		{
			cout << "Sender Connection Nomal close case" << endl;
			Err_Display("recvfrom()");
			break;
		}
        if (recvlen == SOCKET_ERROR)
		{
			cout << "Sender Connection abNomal(RST) close case" << endl;
			Err_Display("recvfrom()");
			break;
		}
		buf[recvlen] = NULL;
		cout << buf << endl;
	}


	closesocket(g_sock);
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
	cout << mes << " : " << err_mes << endl;
	LocalFree(err_mes);
}