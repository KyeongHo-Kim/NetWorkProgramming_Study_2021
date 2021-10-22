## fopen_s()

- 저장된 file을 가져와서 open한다
  
        errno fopen_s
        (
            FILE**       pFile,         // 파일의 맨 처음을 가르킴
            const char  *filename,      // 저장된 파일 이름
            const char  *mod            // 모드 지정 Ex) 읽기..
        )

> 사용예시

        FILE* fptr = NULL;
        fopen_s(&fptr, "file_name", "Mod");
        if(fptr == NULL)
        {
            cout << Not Found File << endl;
            return -1;
        }

</br></br>

## feof()

- file의 끝(마지막)? 을 체크한다.

        int feof
        (
            FILE *stream
        );

</br>

>사용예시
        
        if(feof(fptr))        
        {
            break;
        }