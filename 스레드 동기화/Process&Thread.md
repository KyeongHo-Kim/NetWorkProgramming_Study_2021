## comcurrent(동시성)
- 시분활을 활용한다.(동시에 서비스 하는것처럼 보인다.)

</br>

## Process
- exe file을 실행하면 기본적으로 Process가 할당된다.
  
  class 의 생성자 같은거라고 이해하자

- 운영체제에서 일(작업)을 하는 단위 / 어떤 프로그램을 돌리는것.
- contextswitching - 여러 Process 들이 작업을 번갈가며 하는것을 의미하며, 어떤 Process(A)가 작업을 하다가 멈추고 다른 Process(B) 차례가 끝 날때 까지 기다리게 된다.

     다시 A가 작업을 시작할때 "ProgrammeCounter" 에서 이전에 했던 작업을 가져와 일 한다.

     ProgrammeCounter는 작업내역을 저장한다고 볼 수 있다.

</br>

- text(code)영역, Process와 Processs는 서로 공유할 수 있다. 

    상속관계 개념으로 이해하면 될것 같다.

>정리

어떤 Process(A)가 일을 하다 다른 Process에게 넘겨줄때 contextswitching이라고 한다.

이때 A의 작업내역은 ProgrammeCount에 저장된다. 

다시 A의 차례가 오면 ProgrammeCount에서 부터 일을 시작한다.

Process가 많으면 그만큼 context_switching_Overhead가 많이 발생하기 때문에 Performance가 떨어진다.


</br></br></br>

## Thread
- Process로 부터 만들어진다.
- Text, Data, heep 영역을 공유한다.
- Thread도 ContextSwitching이 발생한다.

</br></br></br>

## _bginthread()
- Thread를 생성한다.
- 정상적으로 작동하면 새로 만들어진 "Thread"의 "Handle"값을 반환한다.
- <process.h> 선언 후 사용가능

      uintptr_t _beginthreadex
      (
        void * security                                       // 파생 프로세스를 만들때 사용 이번학기 사용 x
        unsigned stack_size,                                  // thread에서 공유되지 않는 영역, 직접설정 하지 않고 0으로 두면 default로 설정된다.  이번학기 는 0
        usigned ( __stdcall *start_address )(void *),         // thread를 이용해 일 을 시킬 함수 , 함수의 start address가 들어온다.
        void *arglist,                                        // 사용할 파라미터의 갯수 지정 (배열사용)
        unsigend initfla,                                     // thread를 즉시실행 할지 조건(if)을 걸지 결정 0으로 셋팅하면 기본값으로 설정된다.
        unsigend *thrdaddr                                    // thrdaddr의 ID, thrdaddr를 구분할때 사용(thread를 공유할때 thread ID를 사용한다.)
      );


</br></br></br>

 ## KernelObject
 - kernel resource(커널자원)를 사용하는것.
 - socket과 thread는 kernel resource를 사용한다.
 -  signal 상태와 non_signal상태가 있다.

</br>

>Windows OS

Thread를 생성하면 default "non-signal"상태로 할당된다. Thread가 종료되면 "signal"상태로 변경된다.

</br></br>

## WaitForSingleObject()
- 하나의 Object signal상태를 체크해준다.
- signal상태가 될때 까지 기다린다.

      DWORD WaitForSingleObject
      (
        HANDLE   hHandle              //Thread의 핸들값을 저장한 변수 사용
        DWORD    dwMilliseconds       //얼마나 기다릴지, Infinite 사용
      );
      
-  정상동작 return 0, dwMilliseconds의한 return은 time out, 오류는 Wait_Failed

>사용예시

    HANDLE th;      //Thread의 return값 저장 변수
    
    WaitForSingleObject(th, INFINITE);


</br></br>

## WaitForMultipleObject()
- 여러개의 Object signal상태를 체크해준다.
- signal상태가 될때 까지 기다린다.

      DWORD WaitForMultipleObjects
      (
        DWORD        nCount,          //몇개의 thread를 체크해야할지, SYSTEM_INFO 변수 활용
        const HANDLE *lpHandles,      // Handle값이 저장된 메모리
        BOOL         bWaitAll,        //조건 thread가 전부 (signal)변경됬을때 또는 하나라도 변경되었을때 즉 옵션을 정함.
        DWORD        dwMilliseconds   //기다리는 시간 INFINITE사용.
      );

      bwaitall = true셋팅시 thread가 전부 signal상태가 되어야 빠져나옴 // false 셋팅시 thread가 하나라도 signal상태가 되면 빠져나온다.


</br></br>

## getpeername()

- 상대방의 정보를 얻어온다.

      int getpeername
      (
        SOCKET              s,          // 클라이언트의 소켓
        strucet sockaddr    *name,      // 클라이언트의 정보 (SOCKADDR *)타입
        int                 *namelen    // size 배열사용
      );

 
