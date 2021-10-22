#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

using std::cout;
using std::endl;

void Err_Display(const char*);

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2),&wsa))			
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

	bool broad = true;
	if (setsockopt(g_sock, SOL_SOCKET, SO_BROADCAST, (char*)&broad, sizeof(broad)) == SOCKET_ERROR)		
	{
		Err_Display("setsockopt");
		return -1;
	}

	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);			

	/*if (connect(g_sock,(SOCKADDR*)&saddr,sizeof(saddr)))		
	{
		Err_Display("connect");
		return -1;
	}*/

	FILE* fptr = NULL;
	fopen_s(&fptr, "news.txt", "r");
	if (fptr == NULL)
	{
		cout << "File not found" << endl;
		return -1;
	}

	char buf[80];
	int sendlen;
	while (true)
	{
		fgets(buf, 80, fptr);

		if (feof(fptr))
		{
			break;
		}

		sendlen = strlen(buf);

		sendto(g_sock, buf, sendlen, 0, (SOCKADDR*)&saddr, sizeof(saddr));

		Sleep(500);		
	}

    sendto(g_sock, buf, 0, 0, (SOCKADDR*)&saddr, sizeof(saddr));

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
	cout << mes << " : " << err_mes << endl;
	LocalFree(err_mes);
}