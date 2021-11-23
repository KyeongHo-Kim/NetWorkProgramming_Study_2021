## select()
- 소켓모드(blocking, nonblocking)와 상관없이 여러 소켓을 하나의 스레드로 처리 할 수 있다.
- 연결 요구와 수신데이터를 체크한다.
- listen()전에 작성한다.

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

</br></br>

>사용 예

    fd_set check;               
    FD_ZERO(&check);            
    FD_SET(socket, &check)      
    fd_set check_copy;          
    int ret_select;
    SOCKET s_scok;
    SOCKADDR_IN caddr;
    int caddrlen;
    int recvlen;
    char buf[80];

    while(true)
    {   check_copy = check;     
        ret_select = select(0,&check_copy,NULL,NULL,NULL);
        
        for(int idx = 0; idx < ret_select; idx++)
        {
            if(FD_ISSET(copy_check.fd_array[idx], &copy_check)
            {
                if(copy_check.fd_array[idx] == g_scok)
                {
                    s_scok = accept(g_scok,(SOCKADDR*)&caddr,&caddrlen)
                    if(){Error Check..}
                    FD_SET(s_scok,&check);
                }
                else
                {
                    recvlen = recv(copy_check.fd_arrat[idx],buf,80,0);
                    if(recvlen == SOCKET_ERROR)
                    {
                        closesocket(copy_check.fd_array[idx]);
                        FD_CLR(copy_check.fd_array[idx],&check);
                        break;
                    }
                    if(recvlen == 0)
                    {
                        closesocket(copy_check.fd_array[idx]);
                        FD_CLR(copy_check.fd_array[idx],&check);
                        break;
                    }
                    buf[recvlen] = NULL;
                    cout<<buf<<endl;

                    send(copy_check.fd_array[idx],buf,recvlen,0);
                }
            }
        }
    }
