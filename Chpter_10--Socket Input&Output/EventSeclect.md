## WSACreateEvent()
- 이벤트 객체를 생성한다.
- Default = Manual Reset
- CreateEvent()와 다르게 파라미터가 존재하지 않는다.

>사용예시
        
    EvenArr[obj] = WSACreateEvent();

</br></br>

## WSACloseEvent()
- 생성한 이벤트 객체를 제거한다.

</br></br>

## WSAEvemtSelect()
- 이벤트 오브젝트를 감시한다. 
- 소켓과 이벤트 객체를 하나로 묶는다. 즉 그룹으로 만든다.

        int WSAAPI WSAEventSelect
        (
            [in]    SOCKET        s,                    // 묶을 소켓
            [in]    WSAEVENT      hEventObject,         // 묶을 이벤트 객체
            [in]    long          lNetworkEvents        // 이벤트 종류
        );

        실패: SOCKET_ERROR


        lNetworkEvents 종류

        FD_READ = 읽기, 수신할때 사용

        FD_WRITE = 쓰기, 송신할때 사용

        FD_OOB  = 예외처리 out_of_band

        FD_ACCEPT = 연결요구를 받음

        FD_CLOSE  = 종료




</br></br>

## WSAWaitForMultipleEvents
- 이벤트 객체의 신호 감지, 즉 이벤트 오브젝트가 signald 상태가 될때 까지 기다린다.

        DWORD WSAAPI WSAWaitForMultipleEvents
        (
            [in]    DWORD               cEvents,         // 이벤트 오브젝트의 갯수
            [in]    const WSAEVENT      *lphEvents,      // 이벤트 객체의 그룹 주소 즉 이벤트 객체들이 들어 있는 배열의 주소
            [in]    BOOL                fWaitAll,        // 함수 통과조건, true = 모든 객체가 signal상태일때 빠져나옴 false = 하나라도 signal상태면 빠져나옴
            [in]    DWORD               dwTimeout,       // 시간설정 WSA_INFINTE 사용
            [in]    BOOL                fAlertable       //overlap mod2에서 사용
        );

        timeout에 의해 반환될 경우 WSA_WAIT_TIMEOUT 반환
        정상 반환의 경우 인덱스 번호가 반환되는데 이때 값은 (인덱스 - WSA_WAIT_EVENT_0)값이다.
        
        만약 동시에 signal상태가 되어 반환될 경우 작은 인덱스 값을 반환한다.

        실패시 WSA_WAIT_FAILED 반환

</br></br>

## WSAEnumNetworkEvents
- 해당 소켓에서 어떤 이벤트가 발생했는지 알려주고, 기존에 발생한 이벤트를 저장 후 제거한다.
- 해당 함수가 호출되면 non-signal상태로 변경된다.
  
        int WSAAPI WSAEnumNetworkEvents
        (
            [in]        SOCKET              s,                 // 해당소켓
            [in]        WSAEVENT            hEventObject,      // 해당 이벤트 객체
            [out]       LPWSANERWORKEVENTS  lpNetworkEvent     // 발생한 이벤트를 기록함, 이벤트를 저장할 메모리를 만들어서 사용, WSANETWORKENVETS타입 메모리
        )

        
        typedef struct _WSANETWORKENVETS
        {
            long    lNetWorkEvents;                 // 이벤트의 종류가 들어간다. WSAEventSelect참조
            int     iErrorCode[FD_MAX_EVENT];       // 해당 이벤트가 오류일때, 

        } WSANETWORKEVENTS, *LPWSANETWORKEVENTS;