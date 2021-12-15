#pragma comment(lib,"ws2_32.lib")
#include<iostream>
#include<WinSock2.h>
#include<process.h>
#include<WS2tcpip.h>

using std::cout;
using std::endl;

void Insert_Ip(PCSTR,PVOID);
void Print_IP(LPVOID);
void Err_Display(const char*);
void Add_Session(int);
void Echo_Service(int);
void Close_Session(int);

SOCKET sarr[10];
WSAEVENT earr[10];
int noc;

int main()
{
    WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		Err_Display("WSAStartup()");
		return -1;
	}

	SOCKET g_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (g_sock == INVALID_SOCKET)
	{
		Err_Display("g_sock");
		return -1;
	}

	SOCKADDR_IN saddr;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);

	bool reus = true;
	if(setsockopt(g_sock,SOL_SOCKET,SO_REUSEADDR,(char*)&reus,sizeof(reus)) == SOCKET_ERROR)
	{
		Err_Display("sockopt(reusaddr)");
		return -1;
	}

	if (bind(g_sock,(SOCKADDR*)&saddr,sizeof(saddr)))
	{
		Err_Display("bind()");
		return -1;
	}

	sarr[noc] = g_sock;
	earr[noc] = WSACreateEvent();
	if(WSAEventSelect(sarr[noc],earr[noc],FD_ACCEPT) == SOCKET_ERROR)
	{
		Err_Display("WSAEventSelect");
		return -1;
	}
	noc++;

	if (listen(g_sock,SOMAXCONN))
	{
		Err_Display("listen");
		return -1;
	}

	int ret1,ret2;
	WSANETWORKEVENTS event;
	while (true)
	{
		ret1 = WSAWaitForMultipleEvents(noc,earr,false,WSA_INFINITE,false) - WSA_WAIT_EVENT_0;
		if (ret1 == WSA_WAIT_FAILED)
		{
			cout<<"WSA_WAIT_FAILED"<<endl;
			continue;
		}

		for (int idx = ret1; idx < noc; idx++)
		{
			ret2 = WSAWaitForMultipleEvents(1,&earr[idx],true,0,false);
			if (ret2 == WSA_WAIT_TIMEOUT)
			{
				//cout<<"ERROR: WSA_WAIT_TIMEOUT"<<endl;
				continue;
			}

			if(WSAEnumNetworkEvents(sarr[idx],earr[idx],&event)==SOCKET_ERROR)
			{
				cout<<"WSA_ENUM_ERROR";
				continue;
			}

			if (event.lNetworkEvents & FD_ACCEPT)
			{
				if (event.iErrorCode[FD_ACCEPT_BIT])
				{
					Err_Display("FD_ACCEPT_BIT");
					cout<<"FD_ACCEPT_BIT"<<endl;
					break;
				}

				Add_Session(idx);
				continue;
			}
			
			if (event.lNetworkEvents & FD_READ)
			{
				if (event.iErrorCode[FD_READ_BIT])
				{
					Err_Display("FD_READ");
					cout<<"FD_ACCEPT_BIT"<<endl;
					Close_Session(idx);
					continue;
				}

				Echo_Service(idx);
				continue;
			}

			if (event.lNetworkEvents & FD_CLOSE)
			{
				if (event.iErrorCode[FD_CLOSE_BIT])
				{
					cout<<"abNomal Close Case"<<endl;
					Close_Session(idx);
					break;
				}
				
				cout<<"Nomal Close Case"<<endl;
				Close_Session(idx);
				continue;
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
	cout <<mes<<" : " << (LPSTR)err_mes << endl;
	LocalFree(err_mes);
}

void Insert_IP(PCSTR str, PVOID addr)
{
	if (!InetPton(AF_INET, str, addr))
	{
		Err_Display("InetPton");
		return;
	}
}

void Print_IP(LPVOID addr)
{
	char buf[20];
	if (!InetNtop(AF_INET,addr,buf,20))
	{
		Err_Display("InetNtop");
		return;
	}
	cout << "ip address: " << buf << endl;
}
void Add_Session(int idx)
{
	SOCKADDR_IN caddr;
	int caddrlen = sizeof(caddr);

	SOCKET tcp_sock = accept(sarr[idx],(SOCKADDR*)&caddr,&caddrlen);
	if (tcp_sock == INVALID_SOCKET)
	{
		Err_Display("accept");
		return;
	}

	getpeername(tcp_sock,(SOCKADDR*)&caddr,&caddrlen);
	cout << "Server Conneted by: ";
	Print_IP(&caddr.sin_addr);

	sarr[noc] = tcp_sock;
	earr[noc] = WSACreateEvent();

	if(WSAEventSelect(sarr[noc],earr[noc],FD_READ|FD_CLOSE) == SOCKET_ERROR)
	{
		Err_Display("Add_Session::WSAEventSelect");
		return;
	}
	noc++;
}

void Echo_Service(int idx)
{
	char buf[80];
	int recvlen;

	recvlen = recv(sarr[idx],buf,80,0);
	buf[recvlen] = NULL;
	cout<<"From Client: "<<buf<<endl;

	send(sarr[idx],buf,recvlen,0);
}

void Close_Session(int idx)
{
	SOCKET s = sarr[idx];
	WSAEVENT e = earr[idx];

	if (idx != noc -1)
	{
		sarr[idx] = sarr[noc - 1];
		earr[idx] = earr[noc - 1];
	}

	closesocket(s);
	WSACloseEvent(e);
	
	noc--;
}