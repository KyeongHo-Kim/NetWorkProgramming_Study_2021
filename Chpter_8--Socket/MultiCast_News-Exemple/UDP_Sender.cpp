#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

using std::cout;
using std::endl;

void Err_Display(const char *);
void Insert_IP(PCSTR, PVOID);
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

	int ttl = 16;
	int ttllen = sizeof(ttl);
	if (setsockopt(g_sock, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, ttllen) == SOCKET_ERROR)
	{
		Err_Display("setsockopt_Multicast_TTl()");
		return -1;
	}
	//getsockopt(g_sock, IPPROTO_IP, IP_MULTICAST_TTL,(char*)&ttl,&ttllen);


	SOCKADDR_IN saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	Insert_IP("236.0.0.1", &saddr.sin_addr);

	/*if (connect(g_sock,(SOCKADDR*)&saddr,sizeof(saddr)))
	{
		Err_Display("connect");
		return -1;
	}*/

	FILE *fptr = NULL;
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

		sendto(g_sock, buf, sendlen, 0, (SOCKADDR *)&saddr, sizeof(saddr));

		Sleep(500);
	}

	sendto(g_sock, buf, 0, 0, (SOCKADDR *)&saddr, sizeof(saddr));

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

void Insert_IP(PCSTR str, PVOID addr)
{
	if (!InetPton(AF_INET,str,addr))
	{
		Err_Display("InetPton()");
		return;
	}
}