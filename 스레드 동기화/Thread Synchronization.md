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

</br></br>

## 이벤트

- 대기중인 스레드를 깨운다.
  
- kernel resource를 사용한다.

</br></br>

## 세마포어

- 한정된 자원에 여러 스레드가 접근할때, 자원사용 스레드 갯수를 제한한다.

- kernel resource를 사용한다.


</br></br>

##  대기 기능 타이머

- 정해진 시간이 되면 스레드를 깨운다.

- kernel resource를 사용한다.