# Server Function

</br></br>

## bind() 함수

- socket과  localaddress(IP주소 + port 번호)를 연결한다.
- socket은 localaddress에 관련된 일 들을 처리한다.

        int bind
        (
            SOCKET           s,             //Gate socket(처음 생성한 소켓)
            const sockaddr  *addr,          //ip주소와 port번호 즉 SOCKADDR_IN 타입 메모리를 만들어서 여기 작성 형태는 SOCKADDR* 형태로해야된다.

            int              namelen        //sockaddr  *addr에 해당하는 메모리의 크기(SOCKARRD_IN type memory size)
        );
- 정상적으로 작동하면 return 0, 오류가 발생하면 return socket_error 

</br>

>사용예시
            
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    retval = bind(listen_sock,(SOCKADDR *)&serveraddr,sizeof(serveraddr));
    if(retval == SOCKET_ERROR)
    {
        오류메시지 출력함수("bind()");
    }
    
</br>

> bind()함수를 사용하지않으면?

bind()함수를 사용하지않으면 서버가 클라이언트와 통신을 할때 서버의 컴퓨터에 랜덤한 prot번호가 설정되고 사용가능한 ip주소로 자동 설정된다.

클라이언트 입장에서는 서버의 ip를 알고 있어도 port번호를 모르기때문에 통신이 불가능하다.

</br></br></br>

## listen() 함수

- socket을 연결가능한 상태로 만들어주는 역할을 한다.
- listen_buf에 클라이언트의 요청이 들어오면 연결이 완료된 상태이다.
- 정상적으로 작동하면 return 0, 오류가 발생하면 return socket_error 

      int listen
        (
            SOCKET s,               //Gate socket(처음 생성한 소켓)
            int backlog             //SOMAXCONN(연결요청을 받는 buf를 생성/ (max size listen_buf를 만든다.))
        );


</br></br></br>

## accept()함수

- listen()의 buf에 저장된 요청사항을 가져와 서비스를 한다.
- 연결승인이 아닌 서비스 승인을 담당한다.
- 정상 동작시 listen buf 에서 가져온 요청의 socket discriptor를 반환한다. 즉 특정 클라이언트와 통신을 위한 전용소켓을 새로 만든다.
  
        SOCKET accept
        (
            SOCKET s,               // gate socket
            struct sockaddr*addr,   // SOCKADDR_IN메모리 상대방(클라이언트)의 정보를 저장하기 위한 공간
            int*addrlen             //
        );

        Success = 전용소켓 생성
        failed = INVALID_SOCKET