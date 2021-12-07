# Overlapped IO(중첩 IO) Model-I
- io가 종료됨을 알리는 방법에 따라 달라진다.
</br></br></br>

## WSASocketW()
- Overlapped IO를 지원하는 소켓을 생성한다.


</br></br></br>

## WSARecv()
- Scatter_Gather Io = 다른 메모리에 있는 데이터를 모아서 전송할 수 있다.
- 하나의 수신에 대해 한번만 호출 한다.
- 수신이 완료되지 않아도 리턴된다.
- recv함수가 정상적으로 완료되면 WSAOverLapped::hevent가 signald 상태로 변경된다.

        WSARecv
        (
            [in]        SOCKET                                  s,                      // 어떤 소켓으로 수신할것인지                      
            [in, out]   LPWSABUF                                lpBuffers,              // WSABUF사용, 다른 메모리의에 있는 데이터를 1개 이상 보낼 경우 배열로 셋팅가능
            [int]       DWROD                                   dwBufferCount,          // WSABUF의 카운트 값 버퍼의 갯수에따라 셋팅, 정수값
            [out]       LPDWORD                                 lpNumverOfBytesRecvd,   // 수신된 바이트 수가 들어온다. DWORD 메모리를 만들어서 사용(주소값으로)
            [in, out]   LPDWORD                                 lpFlags,                // 0 셋팅
            [in]        LPWSAOVERLAPPED                         lpOverlapped,           // WSAOVERAPPED 메모리를 만들어서 사용,   ZeroMemory()를 통해 초기화 후 작성해야함
            [in]        LPWSAOVERLAPPED_COMPLETION_ROUTINE      lpCompletionRoutine     // Overlapped IO Model-II에서 사용, 콜백함수 지원
        );

        정상 동작 0 또는 SOCKET_ERROR를 반환한다.
        여기서 SOCKET_ERRPR의 내용은 WSA_IO_PENDING이어야 한다.
        WSA_IO_PENDING은 overlapped operation io 즉 비동기 모델이 정상적으로 동작하고 있다는 의미이다.
        

         -WSABUF-
        typedef struct _WSABUF
        {
            ULONG   len;        // 수신 = 연결된 버퍼를 크기 - 1,   송신 = 보낼 데이터의 크기
            CHAR    *buf;       // 실질적으로 데이터를 받는 버퍼와 연결한다.

        } WSABUF, *LPWSABU

        
        - WSAOVERAPPED -
        반드시 초기화를 하고 사용한다. / ZeroMemory() 사용

        typedef struct _WSAOVERAPPED
        {
            DWORD       Internal;
            DWORD       InternalHight;
            DWORD       Offset;
            DWORD       OffsetHight;
            WSAEVENT    hEvent;                     // 이벤트 객체를 삽입 WSACreateEvents();

        } WSAVOERAPPED, *LPWSAOVERAPPED;

</br></br>

> listen()함수 이전

    Thread를 이용해서 WSAWaitForMultiPleEvents함수를 사용한다.
    Overlapped I/O의 상태(signal / non-signal)를 체크하기위해 필요하다


</br></br>

## WSAGetOverlappedResult()
- 이벤트객체가 어떤 상태인지 판단한다.
- 해당함수가 실행되는 경우는 signald상태가 된  소켓이 존재한다는 의미이다.
- 

        BOOL WSAAPI WSAGetOverlappedResult
        (
            [in]  SOCKET          s,                // signal상태인 소켓
            [in]  LPWSAOVERLAPPED lpOverlapped,     // WSARecv()때 생성한 _WSAOVERAPPED메모리의 주소값 작성
            [out] LPDWORD         lpcbTransfer,     // 송/수신된 바이트 수/ 자동으로 설정/ 송신 또는 수신의 결과를 알 수 있다.
            [in]  BOOL            fWait,            // TRUE = overlapped io가 종료되기 전까지 반환되지 않음
            [out] LPDWORD         lpdwFlags         // 0
        );

        성공 : true         실패 : false

</br></br>

> Session 정보/ ACT(Asynhronous_Communication_Token) 즉 송·수신에 필요한 정보
        
    - WSAOVERALLED 객체                     WSAOVERALLED    over;

    - 소켓정보                              SOCKET          sock;

    - 클라이언트 정보(IP주소, 포트번호)       SCOKADDR_IN     caddr

    - WSABUF(송/수신)                       WSABUF          swsa, rwsa;
    - 실제 송수신 버퍼                       char            sbuf[], rbuf[];
`
    - Session 정보를 처리하는 핸들러         void            (*handler)();      송 수신을 판단하기 위함

    - Event 객체의 핸들값                   WSAEVENT        hevent = WSACreateEvents();

    - 정보를 주거나 가져오기 위함            LPServer        server;              서버로 가기위한 포인터
     (이벤트 핸들값, 소켓정보)

        
        Session생성 시점은 accept()가 완료되어 클라이언트와 통신을 위한 소켓 생성 이후 만들어진다.

</br></br>

> Server 정보

    - Gate_Scoket                              SOCKET               listen_scok;

    - 세션 배열                                LPSession            s_arr[];        vector<LPSession> s_arr;
    - 이벤트 객체 배열                          WSAEVENT             e_arr[];         vector<WSAEVENT>  e_arr;

    - 스레드 동기화를 위한 Critical_Section     CRITICAL_SECTION     cs;

    - 생성된 이벤트객체 생성 수(클라이언트 수)    int                  int noc;

</br></br>

# 전체 순서

</br>

1. Overlapped.h  (struct_session, struct_server) 및 각종 함수 선언
2. Main.cpp = 함수호출 연결 및 송·수신
3. Proactor.cpp(WSAWaitForMultipleEvents(),WSAGetOverlappedResult())
4. util.cpp 각종 초기화

</br>

