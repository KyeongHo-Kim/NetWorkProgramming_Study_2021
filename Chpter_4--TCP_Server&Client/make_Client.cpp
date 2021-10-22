/*
1. winsock을 초기화한다.
    - WSAStartup(MAKEWORD(),&) 

2. Gate_socket을 생성한다.
    - socket(af,type,protocol)

3. ip와 포트번호를 할당한다.
    - SOCKADDR_IN

4. 연결요청을 한다.
    connet(SOCKET,(SOCKADDR*)& ,sizeof())

5. 송신
    - send(SOCKET,buf,szie,option)
    - fgets()

6. 수신
    - recv(SOCKET,buf,szie,option)
*/

#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

using std::endl;
using std::cout;

void Err_Display(const char*);
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

    SOCKET g_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (g_sock == INVALID_SOCKET)
    {
        Err_Display("socket()");
        WSACleanup();
        return -1;
    }

    SOCKADDR_IN caddr;
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(8000);
    Insert_IP("127.0.0.1", &caddr.sin_addr);

    if (connect(g_sock,(SOCKADDR*)&caddr,sizeof(caddr)))
    {
        Err_Display("connet()");
        return -1;
    }

    char buf[80];
    int sendlen;
    int recvlen;
    while (true)
    {
        fgets(buf, 80, stdin);
        sendlen = strlen(buf);
        send(g_sock, buf, sendlen, 0);

        recvlen = recv(g_sock, buf, 80, 0);
        buf[recvlen] = NULL;
        cout << "From Server: " << buf << endl;
    }
 
    closesocket(g_sock);

    WSACleanup();

    return 0;
}

void Err_Display(const char* mes)
{
    LPVOID err_mes;
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
    cout << mes << " : " <<(LPSTR)err_mes << endl;
    LocalFree(err_mes);
}

void Print_IP(LPVOID addr)
{

    char buf[20];
    if (!InetNtop(AF_INET, addr, buf, 20))
    {
        Err_Display("InetNtop");
        return;
    }
    cout << "ip address: " << buf << endl;

}

void Insert_IP(PCSTR str, PVOID addr)
{
    if (!InetPton(AF_INET,str,addr))
    {
        Err_Display("InetPton");
        return;
    }
}
