# Overlapped IO(중첩 IO) Model-I
- io가 종료됨을 알리는 방법에 따라 달라진다.

</br></br></br>

## WSASocketW()
- Overlapped IO를 지원하는 소켓을 생성한다.


</br></br></br>

## WSARecv()
- Scatter / Gather Io = 다른 메모리에 있는 데이터를 모아서 전송할 수 있다.
- 하나의 수신에 대해 한번만 호출 한다.
- 수신이 완료되지 않아도 리턴된다.

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


        정상 동작할경우 0 또는 SOCKET_ERROR를 반환한다.
        여기서 SOCKET_ERRPR의 내용은 WSA_IO_PENDING이어야 한다.
        WSA_IO_PENDING은 overlapped operation io 즉 비동기 모델이 정상적으로 동작하고 있다는 의미이다.

</br>

### WSABUF
    typedef struct _WSABUF
    {
        ULONG   len;        // 수신 = 연결된 버퍼를 크기 - 1,   송신 = 보낼 데이터의 크기
        CHAR    *buf;       // 실질적으로 데이터를 받는 버퍼와 연결한다.

    } WSABUF, *LPWSABUF

</br>

### WSAOVERAPPED
- 반드시 초기화를 하고 사용한다. 
 
    ZeroMemory() 사용

        typedef struct _WSAOVERAPPED
        {
            DWORD       Internal;
            DWORD       InternalHight;
            DWORD       Offset;
            DWORD       OffsetHight;
            WSAEVENT    hEvent;

        } WSAVOERAPPED, *LPWSAOVERAPPED;