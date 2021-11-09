## WSACreateEvent()
- 이벤트 객체를 생성한다.
- Default = Manual Reset
- CreateEvent()와 다르게 파라미터가 존재하지 않는다.

>사용예시
        
    EvenArr[obj] = WSACreateEvent();

</br></br>

## WSACloseEvent()
- 생성한 이벤트 객체를 제거한다.

</br></br>

## WSAEvemtSelect()
- 소켓과 이벤트 객체를 하나로 묶는다. 즉 그룹으로 만든다.
