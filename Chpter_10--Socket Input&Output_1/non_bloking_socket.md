## ioctlsocket()
- "non blocking" socket 
- 교착 상태가 발생하지 않는다 즉 항상 return한다.
- 멀티스레드를 사용하지 않고 동시 서비스를 할 수 있다.
- 함수를 호출 할 때 마다 WSAEWOULDBLOCK와 같은 오류를 처리해야한다.
- 블로킹소켓 보다 cpu사용율이 높다.

        ioctlsocket
        (
            [int]           SOCKET  s,          // gate socket
            [int]           long    cmd,        // FIONBIO = File input output non blocking input output
            [int, out]      u_long *argp        //
        );

>사용예시

    u_long on = 1;    
    ioctlsocket(socket, FIONBIO, &on); 