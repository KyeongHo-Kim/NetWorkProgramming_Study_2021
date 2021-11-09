## select()
- 소켓모드(blocking, nonblocking)와 상관없이 여러 소켓을 하나의 스레드로 처리 할 수 있다.

        int WSAAPI select
        (
            [int]       int              nfds,          // windos경우 무시함 / Lenux의 경우 소켓센싱 갯수
            [in, out]   fd_set           *readfds,      // 읽기를 원하는 socket의 discriptor 값을 모아둔 그룹    / 값이 반환되면 최종적으로 하나의 socket만 남게됨
            [in, out]   fd_set           *writefds      // 송신이 완료 되었을때 감지함
            [in, out]   fd+set           *exceptfds,    // 예외처리, out of bound data
            [in]        const timeval    *timeout       // NULL로 셋팅시 무한대기
        );
        정상: 읽을 준비가 된 소켓값의 갯수를 반환한다.(연결요구 / 수신받은 데이터가 있다) 
        오류: SOCKET_ERROR


</br></br>

## FD_SET
- 소켓의 갯수와 각각 소켓들의 주소값을 갖는다.
- fd-set 타입의 메모리가 필요하다.

        typedef struct fd_set
        {
            u_int   fd_count;                       // 읽을 소켓의 갯수
            SOCKET  fd_array[FD_SETSIZE];           // 인덱스 별 각 소켓에 해당하는 주소값

        } fd_set, Fd_SET, *PFD_SET, LPED_SET;

</br></br>

## FD_ZERO
- 초기화 목적으로 사용한다.

</br></br>

## FD_CLR
- socket을 제거한다.

</br></br>

## FD_ISSET
- 해당 소켓이 메모리에 있는지 판단한다.