# winSock 초기화와 종료


## WSAStartup()

- winScok을 초기화한다.


        int WSAAPI WSAStartup
        (
            WORD wVersionRequested,     //MAKEWORD()사용 
            LPWSADATA lpWSAData     //포인터이며(1개), WSADATA가 들어간다.
        )
- return value가 0이면 정상 아니면 error code가 나온다.
  
</br>

>MAKEWORD() 버전을 작성할때 major를 먼저쓰고 minor를 쓸 수 있도록 해주고, lowByte = major / highByte = minor 형태로 자동 변환한다.

</br></br>

>사용 예

        WSADATA wsa;                            // WSADATA 타입의 변수생성
        if(WSAStartup(MAKEWORD(2,2),&wsa))      // winSock 초기화
            return -1;                          //Error면 종료
  
         
</br></br>


## WSACleanup()
- winSock을 종료한다.
    
        WSACleanup(void)    //파라미터 없음

</br>

>사용 예

        WSACleanup();

</br></br></br></br>


# 소켓 생성과 닫기

</br>

## socket()
- 소켓을 생성한다.

        SOCKET socket
        (
                int addrs_family        // 주소 체계, AF_INET 작성
                int type                // 소켓 타입, SOCK_STREAM or SOCK_DGRAM
                int protocol            // 프로토콜, IPPROTO_TCP or IPPROTO_UDP
        )
                                성공 새로운 소켓 desciptor값 반환(번호가 리턴됨)
                                실패 NVALID_SOCKET(소켓 생성에 관련한 오류)

</br>

### address_family
- AF_INET = ip v4
- AF_INET6 = ip v6
- AF_IRDA = 적외선
- AF_BTH = 블루투스

</br>

### sockType
- SOCK_STREAM = 신뢰성 있는 데이터 전송, 연결형 프로토콜/ Ex) TCP
- SOCK_DGRAM = 신뢰성 없는 데이터 전송, 비연결형 프로토콜/ Ex) UDP

</br>

>사용 예

        SOCKET tcpSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)        // SOCKET타입의 tcpsock변수를 만들고 socket함수를 넣는다.
        if(tcpSOCK == INVALID_SOCK)                                     // Error의 경우 
        {
                err_display("socket")                                   // formatmessage 함수 호출
                WSACleanup();
                return 0;                                               // 종료
        }
        cout<<"socket discriptor : "<< tpcsock <<endl;                  // 정상일때 출력


</br></br></br>

## closesocket()
-소켓을 닫는다(반환한다)

        int colsesocket
        (
                SOCKETs
        )

</br>

>사용 예

        closesocket(tcpsock);           //소켓 생성때 만든 변수를 파라미터로 설정
