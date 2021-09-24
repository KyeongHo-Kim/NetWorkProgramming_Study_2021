# 소켓 주소 구조체

</br></br>

## Ip주소와 Port번호 지정하기

</br>

> 기본형

    typedef struct sockaddr     //기본 구조체
    {
        u_short as_family;      // 주소체계
        char as_data[14];       // 
    }SOCKADDR;                  // 명칭(이름)

</br></br>

> IPv4 전용

        typedef struct sockaddr_in  
        {
            short sin_family;           // AF_INET
            u-short sin_prot;           // 포트번호
            srtuct in_addr sin_addr;    // ip주소
            char sin_zerop[8];          // 0으로 설정
        } SOCKADDR_IN;

</br>

## 바이트 정렬 함수 

> ### 네트워크 바이트 정렬
 - 네트워크를 사용하여 데이터나 정보를 송신할때는 network byte order 변환
 - 소켓 구조체의 port번호를 작성할때 사용
  
        u_short htons       //host-to-network-short
        u_long htonl        //host-to-network-long

</br>

> ### 호스트 바이트 정렬
- 수신받은 데이터(ip주소 & port번호)를 출력할려면 host byte order 변환 

        u_short ntohs       //network-to-host-short
        u_long ntohl       //network-to-host-long

</br></br>

## IP주소 작성 (IN_ADDR 구조체)
>union의 size는 제일 큰 것으로 통일된다. 즉 'N'byte가 여러개 있어도 메모리의 size는 제일 큰 'N'byte의 size로 결정된다.

</br>

- #include<ws2tcpip.h>선언 후 사용가능
  
</br>

    typedef struct in_addr                              // 메모리의 size는 4byte
    {
        union
        {
            struct {u_char b1,b2,b3,b4} S_nn_b;         //1번 1byte *4
            struct {u_short w1, w2} S_nn_W;             //2번 2byte *2
            u_long addr;                                //3번 4byte 1
        }S_un;                                          

    #define s_addr S_un.S_addr                          //s_addr을 사용하면 S_un(union)의 3번형태로 저장해준다.

    }IN_ADDR;

</br>

>InetPton()
- 문자열 형태의 IPv4 or IPv6 주소를 숫자 바이너리로 변환한다. 
        
        INT WSAAPI InetPton
        (
            INT      Family,            // 주소체계
            PCSTR    pszAddrString,     // 1. ip형태로 되어있는 문자열 작성 Ex)"1.2.3.4" , 1-2. NULL로 끝나야됨
            PVOID    pAddrBuf           // 2. 1에서 설정한 ip주소를 network byte order형태로 저장
        );
- 정상적으로 작동하면 1을 리턴 다른 값은 Error

</br></br>

> InetNtop()
- IPv4 or IPv6 network address를 string 형태로 변환한다.
        
        PCSTR WSAAPI InetNtop
        (
            INT     Family,         // 1. 주소체계
            const VOID *pAddr,      // 2. network byte order의 메모리 saddr.sin_addr사용
            PSRT    pStringBuf,     // 3. 16byte이상 의 메모리공간 필요, 메모리를 만들고 여기에 사용
            size_t  StringBufSize   // 4. 3번에 사용된 메모리의 size작성
        );
- 정상적으로 작동하면 포인터를 리턴히고 error가 발생하면 NULL-point가 리턴된다.

</br></br>

## 도메인네임 -> IP주소 변환

>getadddrinfo()

    INT WSAAPI getaddrinfo
    (
        PCSTR   pNodeName,              // 문자열 형태로 host domain name을 작성 Ex)"naver.com"
        PCSTR   pServiceName,           // 문자열 형태로 작성 Ex) "http" or "80"
        const ADDRINFOA    *pHints,     // DNS Server에 힌트를 주는것, 
        PADDRINFOA  *ppResult           //
    );




>순서

1. SOCKADDR_IN 타입의 메모리 즉 변수를 생성한다.
2. 생성된 변수에 ip주소와 port번호를 넣는다.
3. socket함수에 넘겨줄때 주소값을 사용하여 알려주는데 이때 포인터의 형식은 "기본형"인 SOCKADDR로 설정한다.


</br></br></br>

</br></br></br>





- socket함수를 (SOCKADDR/기준구조체)사용하여 상대방의 포트번호와 ip주소를 요구하는 형식
