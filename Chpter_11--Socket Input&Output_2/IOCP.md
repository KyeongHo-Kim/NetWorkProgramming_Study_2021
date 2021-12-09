# IOCP : I/O Completion Port

IOCP가 생겨난 배경
싱글 스레드를 사용하여 프로그램을 작성하면, 클라이언트가 여러개일 경우 하나의 서비스가 끝나기 전까지는 기다려야 하는 문제점이 발생

이런 문제가 발생하는 이유는 여러가지가 있다.
1. 블로킹 모드 함수 accept / recv함수 등 네트워크 io가 발생하지 않으면 아무런 동작을 하지 않음

이런 문제를 해결하기위해 멀티 스레드를 사용했음.

스레드의 갯수는 context switching overhead가 발생하지 않을 갯수만큼 사용
msdn 기준 cpu core갯수 만큼 생성

메모리 풀 : 미리 일정량의 메모리를 만들어 두는것
쓰레드 풀 : 일정량의 풀을 만들어 스레드를 배치? 저장한 뒤 walk가 발생하면 
스레드를 하나씩 불러 walk를 처리 하게 한다. walk가 종료되면 다시 pull로 돌아간다.

이런 과정을 iocp가 제공해준다.

요약 서버는 멀티스레드가 필요하다
스레드의 적당량은 msdn기준 pc의 coer갯수 * 2개 만큼 생성하는게 효율적이다.

스레드를 효율적으로 돌리기 위해서 스레드 풀이 필요하다.
여기서 스레드들이 walk를 동시에 access하더라도 동기화를 지원해야한다.

recv의 size를 0으로 설정하면 pumping 단계가 줄어든다.
pazelock락 문제가 발생할 경우 zeor byte를 read하여 해결 가능하다.

iocp는 5개의 자료구조가 있다.

iocp 최초생성
CreateIoCompletionPort
(
    소캣 핸들값 최초 생성시 INVALID_HANDLE_VALUE
    존재하는 iocp port 처음이면 NULL   
    NULL    //키 값
    스래드의 갯수 0 셋팅시 자동적으로 지원해주는 만큼 셋팅됨
)

accept에 의해 새로운 소켓이 만들어 지면 iocp에 등록한다.

CreateIoCompletionPort
(
    (HANDLE)accept socket   핸들값으로 사용해야 한다.
    최초로 만든 iocp 포트를 쓴다.
    키 값
    스레드의 갯수 
)

위 함수가 호출 되면
device List가 만들어 진다
여기 리스트에는
accept_sock(hDevice), 적당한 키 값(dwCompletionkey)

device List는 accept_sock이 close되면 사라진다.

io가 완성이 되면 i/o completion Queue(FIFO)

completion Queue에서 데이터를 pop하는 함수는
GetqueuedCompletionstatus

thread pool

순서
1. iocp 생성
2. accept 진행
3. 새로운 소켓과 iocp를 bind한다
4. thread 생성
5. 각 스레드들은 GetqueuedCompletionstatus함수를 호출


