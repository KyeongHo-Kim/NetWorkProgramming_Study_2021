## SocketOption - General

</br>

### Level = SOL_SOCKET

</br>

> SO_BROADCAST
-  Type = bool, 브로드캐스팅 허용 여부

</br>

>SO_KEEPALIVE
- Type = bool, 연결상태를 주기적으로 확인한다. (클라이언트가 확실히 연결 되어 있는지 판단 연결x 상태이면 Session을 끊는다.)

</br>

>SO_LINGER
- Type = struct(구조체)LINGER, closesocket을 호출하고 언제 끝이 나는지 알고 싶을때 사용
- server입장에서 client와 강제로 연결을 끊고 싶을때 사용한다. 예를 들어 alive message가 일정 시간이 지나도 오지않거나, 특정 클라이언트를 강제로 끊어 버릴때 사용한다.

        LINGER ling;
		ling.l_onoff = 1;       //linger option 사용 여부 1 = true 0 = false
		ling.l_linger = 0;      // time out, 보통 0으로 셋팅해서 reset signal을 날린다.


</br>

>SO_SNDBUF, SO_RCVBUF
- Type = int, 소켓 송·수신 buf의 크기를 설정할 수 있다.


</br>

>SO_SNDTIMEO, SO_RCVTIMEO
- Type = int, 데이터 송·수신 함수 호출 시 time out 설정 가능

</br>

>SO_REUSEADDR
- Type = bool, LocalAddress 재사용 허용 여부
- bind Error를 방지할 수 있다.

</br></br></br>

## SocketOptopion - Protocol Dependent

</br>

### Lavel = IP_PROTO

</br>

> IP_HDRINCL
- Type = bool, 추가 필요

</br>

>IP_TTL
- Type = int, 추가 필요

</br>

> IP_MULTICAST_IF
- Type = IN_ADDR{}(구조체), LAN CARD가 2개 일때 하나를 멀티케스트 담당 카드로 설정한다.

>IP_MULTICAST_TTL
- Type = int, 멀티캐스트의 ttl값을 설정한다. 다른 네트워크의 host와 통신할때 chack필요

</br>

>IP_MULTICAST_LOOP
- Type = bool, 멀티캐스트 패킷의 루프백 여부 즉, 내가 보낸 data를 내가 다시 받는 것을 허용 할지 말지 결정

</br>

>IP_ADD_MEMBERSHIP, IP_DROP_MEMBERSHIP 
- Type = IP_MREQ(구조체), 멀티캐스트 그룹 가입과 탈퇴

>사용예시

	IP_MREQ mreq;	// IP_MREQ타입 메모리 생성

	InetPton(AF_INET,"자신의 ip",&mreq.imr_interface);	// 자신의 ip 설정
	InetPton(AF_INET,"멀티캐스트 ip",&mreq.imr_interface);	// 멀티캐스트 ip 설정