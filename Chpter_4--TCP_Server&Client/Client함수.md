# Client Fucntion

</br></br>

## connect() 함수
- Server에 연결 요구를 한다.
        
      int connect
      (
          SOCKET                    s,
          const struct sockaddr     *naem,      // SOCKADDR_IN 서버의 주소 (ip, port)
          int                       namelen
      );

- 정상작동 0반환 실패 SOCKET_ERROR


## fgets()함수
- stream에서 문자열을 읽는다.
- 첫 번째 개행 문자를 포함하는 곳까지 읽거나, n−1개의 문자를 읽는다.
- stdin or stdout은 standard input output을 의미한다.

        char* fgets (char *buf, int n, FILE *stream)

>사용예시

    char buf[80]     //입력 data 저장공간
    
    fgets(buf,80,stdin);

