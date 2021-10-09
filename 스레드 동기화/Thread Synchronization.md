# Thread Synchronization

</br></br>

## 임계영역

  - 공유 자원에 대해 오직 하나의 스레드만 접근 허용

    (한 프로세스에 속한 스레드 간에만 사용 가능)
  - kernel resource를 사용하지 않는다.

</br></br>

## 뮤텍스

- 공유 자원에 대해 오직 하나의 스레드만 접근 허용

    (서로 다른 프로세스에 속한 스레드 간에도 사용 가능)
- kernel resource를 사용한다.

- Thread가 죽으면 자동적으로 signal 상태로 변환된다.

>Kernel Object를 만든다.

      HANDLE CreateMutex
      (
          LPSECURITY_ATTRIBUTES   lpMutexArrtibutes,   //  상속을 할지 말지 결정 
          BooL                    bInitialOwner,       //  true = nonsignal false = signal
          LPCWSRT                 lpName               //  별명을 만들 수 있다. 참조변수로 이해하자 이번학기 사용x
      );

      HANDLE값을 반환한다.

</br></br>

## 이벤트

- 대기중인 스레드를 깨운다.
  
- kernel resource를 사용한다.

- Auto Reser과 Manual Reset을 정할 수 있다.

</br></br>

## 세마포어

- 한정된 자원에 여러 스레드가 접근할때, 자원사용 스레드 갯수를 제한한다.

- kernel resource를 사용한다.


</br></br>

##  대기 기능 타이머

- 정해진 시간이 되면 스레드를 깨운다.

- kernel resource를 사용한다.