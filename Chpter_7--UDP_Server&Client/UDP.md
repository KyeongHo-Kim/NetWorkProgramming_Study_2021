## TCP와 차이점

        | TCP |         | UDP |
        ------------------------
        연결형          비연결형
        ------------------------
        신뢰성 O         신뢰성 X
    데이터 재전송 O    데이터 재전송 X
        ------------------------
        1:1          1:1 , 1:다수
        ------------------------
    packet 형태로       한번에 다 전송
    분할 전송           


</br></br>

## Multicast
- 특정 그룹을 지정해서 해당 네트워크에 연결된 host에게 data를 전송하는것.

</br></br>


## UDP의 함수
- 연결과정(Session)이 없기 때문에 받거나 보낼때 목적지의 ip 주소와 port 번호를 포함해야 한다.
- "Server" =  listen(), accept() 함수 사용 X
- "Client" = connect(), 기본적으로 사용X / 특정상황에 사용함
- 하나의 socket으로 여러 클라라이언트를 담당할 수 있다.


</br></br>

>기본적으로 TCP의 함수와 비슷하다 send recv에 목적지 ip 와 port번호만 추가됨

## sendto()

    int sendto
    (
        SOCKET s,
        const char  *buf
        int len,
        int flags,
        const struct sockaddr *to,      //목적지의 주소 SOCKADDR_IN 메모리 만들어서 사용
        int tolen   //위에서 만든 메모리의 크기
    );

    성공 : 보낸 바이트  수  /   실패 : SOCKER_ERROR


</br></br>

## recvfrom()

    int recvfrom
    (
        SOCKER s,
        char *buf,
        int len,
        int flags,
        struct sockaddr *from,      // 송신자의 정보를 저장할 메모리 SOCKADDR_IN 메모리 만들어서 사용
        int *fromlen    //위에서 만든 메모리의 크기 send와 다르게 주소값임
    );

    성공 : 받은 바이트 수 /  실패 : SOCKER_ERROR 

</br></br>

## setsockopt()
- socket의 option을 설정하고 변경한다.

</br>

>Broadcast 
- 동일한 네트워크에 연결된 모든 host에게 data를 전송하는것 

        setsockopt
        (
            SOCKET          s,              // gate socket
            int             level,          // socket option 대 분류 해놓은것
            int             optname,        // socket name 위에서 분류한 옵션의 이름
            const char      *optval,        // optname을 변경하기 위한 주소값 반드시(char*)형 변환 후 사용
            int             optlen          //  size
        );

        문제가 발생하면 SOCKER_ERROR 반환

>사용예시

