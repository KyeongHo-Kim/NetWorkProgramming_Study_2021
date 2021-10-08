# 소켓 주소 구조체

</br></br>

## Ip주소와 Port번호 지정하기

</br>

> 기본형
- sand(송신) 함수에 IP와 port번호를 전달할때 대표로 사용한다.

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

## SOCKADDR_IN구조체의 IP주소 작성 (IN_ADDR 구조체)
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
- 입력할때 사용
        
        INT WSAAPI InetPton
        (
            INT      Family,            // 주소체계
            PCSTR    pszAddrString,     // 1. ip형태로 되어있는 문자열 작성 Ex)"1.2.3.4" ,문자열형태를 숫자로 바꿔줌
            PVOID    pAddrBuf           // 2. 1에서 설정한 ip주소를 network byte order형태로 저장
        );
- 정상적으로 작동하면 1을 리턴 다른 값은 Error

</br></br>

> InetNtop()
- IPv4 or IPv6 network address를 string 형태로 변환한다.
- 출력할때 사용
        
        PCSTR WSAAPI InetNtop
        (
            INT     Family,         // 1. 주소체계
            const VOID *pAddr,      // 2. network byte order의 메모리 saddr.sin_addr사용
            PSRT    pStringBuf,     // 3. 16byte이상 의 메모리공간 필요, 메모리를 만들고 여기에 사용
            size_t  StringBufSize   // 4. 3번에 사용된 메모리의 size작성
        );
- 정상적으로 작동하면 포인터를 리턴히고 error가 발생하면 NULL-pointer가 리턴된다.

</br></br>

## Domain Name 형태로 사용

>getadddrinfo()

    INT WSAAPI getaddrinfo
    (
        PCSTR   pNodeName,              // 문자열 형태로 host domain name을 작성 Ex)"naver.com"
        PCSTR   pServiceName,           // 문자열 형태로 작성 Ex) "http" or "80"
        const ADDRINFOA    *pHints,     // DNS Server에 힌트를 주는것, 메모리를 만들어야됨 / 구조체 사용
        PADDRINFOA  *ppResult           // 결과를 저장할 공간필요 PADDRINFOA타입의 메모리공간을 만들고 초기화시킨다.
    );

</br>

> getaddrinfo의 hints 구조체 형식

- gaddrinfo()의 hints를 작성할려면 아래와같은 형식의 메모리를 생성해야한다.
- 반드시 0으로 초기화한다

        ADDRINFOA hints;                            //DNS에 제공할 힌트
        ZeroMemory(목적지, 사이즈)                   //hints 초기화
        PADDRINFOA result = NULL;                   //결과값을 받을 메모리(포인터)

        typedef struct addrinfo
        {
            int                  ai_flags;          // AI_CANONNAEME 작성 
            int                  ai_family;         // AF_INET
            int                  ai_socktype;       //Tcp = STREAM      Udp = DGRAM
            int                  ai_protocol        //IPPROTO_???

            size_t               ai_addrlen;
            char                 *ai_canonneame;
            struct sockaddr      *ai_addr;          //sockaddr타입의 메모리를 만들고 size를 ai_addrlen에 알려준다.
            struct addrinfo      *ai_next;          //ip주소가 여러개일때 다음 addrinfo memory

        }ADDRINFOA


## Linkd list(연결형 리스트)

>기본지식

1. 포인터 메모리를 만들면 타입과 상관없이 32bit 이다.
2. 포인터 메모리에 어떤 값을 넣기위해선 타입이 같아야한다. (형 변환필요)


</br></br>

>순서

1. SOCKADDR_IN 타입의 메모리 즉 변수를 생성한다.
2. 메모리 형식에 맞추어 주소체계, 포트번호, ip주소를 작성한다.

  - ip주소 작성법
    
        dotted decmal

        if(InetPton(AF_INET,"ip주소", &SOCKADDR_IN타임 메모리 주소) != 1)
        {
                error메시지 출력함수
                return -1;
        }

        domin name

        ADDRINFOA hints = NULL;
        ZeroMemory(목적지,사이즈)
        PADDRINFO result = NULL;

        hints.ai_flags
        hints.ai_family
        hints.ai_socketype
        hints.ai_prtocol
        
        if(getaddrinfo("도메인네임","http",&hints,&result))
        {
            error 메시지 출력함수
            return -1
        }

        PADDRINFOA ptr = NULL;
        SOCKADDR_IN *qtr = NULL;

        for(ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {
            qtr = (SOCKADDR_IN*)(ptr->ai_addr);
            ip설정 확인용 함수(&qtr);
        }