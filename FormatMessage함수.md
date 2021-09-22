## WSAGetLastError
- 소캣함수를 실행하여 실패했을때 어떤 Error인지 알 수 있다.
- 정수, 즉 숫자로 Error를 알려준다.
- 비동기로 사용한다.

</br></br></br>

## Format Message
- Error Code(정수)를 문자로 해석해준다.
- 자체적으로 메모리를 만든다.
- 값을 point즉 주소값으로 넘겨준다 (start addres)

</br>

>size가 정해지지않고 변하는 경우 **포인터 2개를 사용한다.

    void F(char **b)        //3. a의 주소가 복사되어 넘어옴
    {
        char c = "Q";       //4. 변수 c를 만들고 Q를 넣음
        *b = &c;            //5. *b(현재 a의 주소가 들어있음)의 값을 C의 주소값으로 변경
    }

    int main()
    {
        char *a = NULL;     //1. 포인터변수 a를 만들고, NULL로 초기화

        F(&a);              //2. a의 주소를 넘김

        cout<< a <<endl;    //6. Q출력

        return 0;
    }

    
</br></br>

    DWORD FormatMessage
        (
            DWORD dwFlags       // 옵션, FORMAT_MESSAGE_ALLOCATE_BUFFER : formatmessage를 저장할 수 있는 버퍼(저장공간)를 만듬
                                //       FORMAT_MESSAGE_FROM_SYSTEM : SYSTEM에서 제공하는 error code를 찾는다.
            LPCVOID lpSource    // NULL, system에서가 아닌 내가 직접 Error Code Table을 만들경우 사용 아니면 null
            DWORD dwMessageID   // 오류코드, WSAGetLastError함수를 작성
            DWORD dwLanguageId  // 언어, MAKELANGID(LANG_NEUTRAL SUBLANG-DEFAULT)함수사용, 어떤언어로 사용할지..
            LPISTR lpBuffer     // 오류 문자열 시작 주소, 만들어진 버퍼의 주소값(start addres)을 넣음
            DWORD nSize         //0 , lpbuffer의 size를 제한하고 싶을경우 작성 아니면 0
            va_list *Argumant   // NULL, 특별한 파라미터를 사용할경우 작성.
        );

</br>

>사용예제

    #pragma comment(lib,"ws2_32.lib")
    #include<winSock2.h>
    #include<iostream>

    using std::endl;
    using std::cout;

    void err_disPlay(const char *mes)
    {
        LPVOID err_mes = NULL;		//포인터 변수 생성

        //FormatMessage함수 작성
        FormatMessage
        (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	//dwFlags
            NULL,								//lpSource
            WSAGetLastError(),					        //dwMessageId
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),		        //dwLanguageId
            (LPSTR)&err_mes,						//lpBuffer
            0,								//nSize
            NULL								//list	Argument
        );
        
        cout << mes<<" : "<< (LPVOID)err_mes << endl;
        LocalFree(err_mes);
    }

    int main()
    {
        return 0;
    }

