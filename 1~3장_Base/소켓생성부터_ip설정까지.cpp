#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>

using std :: endl;
using std :: cout;
using std :: cin;

void Err_Display(const char *);
void print_IP(LPVOID); 

int main()
{
    //1. winsock을 초기화한다.
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa))
    {
        Err_Display("WSAStartup()");
        return -1;
    }


    //2. socket을 생성한다.
    SOCKET tcpSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    //2-1 오류가 발생할경우
    if (tcpSock == INVALID_SOCKET)
    {
       Err_Display("socket()");
       WSACleanup();
       return 0;
    }
    //2-2 정상출력 확인
    cout<<"socket_discriptor : "<<tcpSock<<endl;



    //3. ip주소와 port번호 셋팅 및 확인 출력
    SOCKADDR_IN iPv4_addr;

    iPv4_addr.sin_family = AF_INET;
    iPv4_addr.sin_port = htons(8000);

    //3-1 dotted decimal형식으로 설정
    // if(InetPton(AF_INET,"1.2.3.4",&iPv4_addr.s   r) != 1)
    // {
    //     Err_Display("InetPton()");
    //     return -1;
    // }

    //3-2 Dommin Name형식으로 설정
    ADDRINFOA hints;
    ZeroMemory(&hints,sizeof(hints));
    PADDRINFOA  result = NULL;

    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo("www.google.co.kr","https",&hints,&result))
    {
        Err_Display("getaddrinfo()");
        return -1;
    }
    
    //3-2-1 연결된 리스트 추적
    PADDRINFOA ptr = NULL;
    SOCKADDR_IN *v4_ptr = NULL;
    for (ptr = result; ptr != NULL; ptr=ptr->ai_next)
    {
        v4_ptr = (SOCKADDR_IN*)(ptr->ai_addr);
        print_IP(&v4_ptr);
    }
    
    

    //??. socket을 닫는다.
    closesocket(tcpSock);



    //??. winsock을 닫는다.
    WSACleanup();
    

    return 0;
}


// 오류 메시지 출력 함수
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

    cout<< mes <<" : "<<(LPSTR)err_mes<<endl;
    LocalFree(err_mes);
}


//설정된 ip확인 함수
void print_IP(LPVOID addr) 
{
    char v4_Buf[20];
    if (!InetNtop(AF_INET, addr, v4_Buf,20))
    {
        Err_Display("InetNtop()");
        return;
    }
    cout<<"ip address: "<<v4_Buf<<endl;  
}