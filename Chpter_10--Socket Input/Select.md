## select()
- 소켓모드(blocking, nonblocking)와 상관없이 여러 소켓을 하나의 스레드로 처리 할 수 있다.

        int WSAAPI select
        (
            [int]       int              nfds,          // windos경우 무시함
            [in, out]   fd_set           *readfds,      // 읽기를 원하는 socket의 discriptor 값을 모아둠    /  최종적으로 하나의 socket만 남게됨
            [in, out]   fd_set           *writefds      // 송신이 완료 되었을때 감지함
            [in, out]   fd+set           *exceptfds,    // 예외처리, out of bound data
            [in]        const timeval    *timeout       // NULL로 셋팅시 무한대기
        );