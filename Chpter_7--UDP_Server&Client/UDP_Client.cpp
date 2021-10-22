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
	Insert_IP("127.0.0.1",&saddr.sin_addr);
	//saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*if (connect(g_sock,(SOCKADDR*)&saddr,sizeof(saddr)))
	{
		Err_Display("connect");
		return -1
	}*/
	
	char buf[80];
	int sendlen;
	int recvlen;
	int saddrlen = sizeof(saddr);
	while (true)
	{
		fgets(buf, 79, stdin);
		sendlen = strlen(buf);
		sendto(g_sock, buf, sendlen, 0, (SOCKADDR *)&saddr, saddrlen);

		recvlen = recvfrom(g_sock, buf, 80, 0, (SOCKADDR *)&saddr, &saddrlen);
		buf[recvlen] = NULL;
		cout << "From Server: " << buf << endl;

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