# 소켓 주소 구조체

</br></br>

## Ip주소와 Port번호 지정하기

</br>

> 기본형

    typedef struct sockaddr     //기본 구조체
    {
        u_short as_family;      // 주소체계
        char as_data[14];       // 
    }SOCKADDR;                  // 명칭(이름)

</br></br>

> IPv4 전용

        typedef struct sockaddr_in
        {
            short sin_family;           // AF_INET
            u-short sin_prot;           // 포트번호
            srtuct in_addr sin_addr;    // ipv4주소로 설정
            char sin_zerop[8];          // 0으로 설정
        } SOCKADDR_IN;

</br></br>

>순서

1. SOCKADDR_IN 타입의 메모리 즉 변수를 생성한다.
2. 생성된 변수에 ip주소와 port번호를 넣는다.
3. socket함수에 넘겨줄때 주소값을 사용하여 알려주는데 이때 포인터의 형식은 "기본형"인 SOCKADDR로 설정한다.






</br></br></br></br>





- socket함수를 (SOCKADDR/기준구조체)사용하여 상대방의 포트번호와 ip주소를 요구하는 형식
