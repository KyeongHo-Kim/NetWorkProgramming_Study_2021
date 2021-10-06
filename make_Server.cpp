/*
1. winsock을 초기화한다.
    - WSAStartup()
2. Gate_Socket을 생성한다.
    - SOCKET name = socket()
3. localaddress와 socket을 연결한다.
    - SOCKADDR_IN name;
    - bind()
    - return 0이 아니면 Error
4. 클라이언트로 부터의 요청을 연결상태로 만든다.
    - listen()
    - SOMAXCONN
    - return 0이 아니면 Error
5. 클라이언트와의 서비스를 승인한다.
    - accept()
*/

#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

using std::endl;
using std::cout;

void Err_Display(const char *);
void Print_IP(LPVOID);

int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa))
    {
        Err_Display("WSAStartup()");
        return -1;
    }
    
    SOCKET s_Sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (s_Sock == INVALID_SOCKET)
    {
        Err_Display("s_Sock");
        WSACleanup();
        return 0;
    }
    
    SOCKADDR_IN v4_addr;
    v4_addr.sin_family = AF_INET;
    v4_addr.sin_port = htons(8000);
    // if (InetPton(AF_INET,"0.0.0.0",&v4_addr.sin_addr) != 1)
    // {
    //     Err_Display("InetPton()");
    //     return -1;
    // }
    v4_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(s_Sock,(SOCKADDR*)&v4_addr,sizeof(v4_addr)))
    {
        Err_Display("bind");
        return -1;
    }

    if (listen(s_Sock,SOMAXCONN))
    {
        Err_Display("listen()");
        return -1;
    }
    
    SOCKADDR_IN caddr;
    ZeroMemory(&caddr,sizeof(caddr));
    int caddrlen = sizeof(caddr);

    SOCKET c_scok = accept(s_Sock,(SOCKADDR*)&caddr,&caddrlen);
    if (c_scok == INVALID_SOCKET)
    {
        Err_Display("accept()");
        return -1;
    }
    
    

    return 0;
}

void Err_Display(const char *mes)
{
    LPVOID err_mes;
    FormatMessage
    (
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
        (LPSTR)err_mes,
        0,
        NULL
    );
    cout<<mes<<" : "<<err_mes<<endl;
    LocalFree(err_mes);
}

void Print_IP(LPVOID addr)
{

    char buf[20];
    if (!InetNtop(AF_INET,addr,buf,20))
    {
        Err_Display("InetNtop");
        return;
    }
    cout<<"ip address: "<<buf<<endl;
    
}