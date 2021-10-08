#include<iostream>
#include<WinSock2.h>
#include<process.h>

using std::endl;
using std::cout;

CRITICAL_SECTION cs;                        //1. 임계영역 key 생성

unsigned __stdcall Func(LPVOID);

int counter;

int main()
{
    int arg[] = {0,10000};
    HANDLE th[20];

    InitializeCriticalSection(&cs);         //2.  key 초기화

    SYSTEM_INFO info;
    GetSystemInfo(&info);

    clock_t stime = clock();

    for(int i = 0; i < info.dwNumberOfProcessors; i++)
        th[i] = (HANDLE)_beginthreadex(NULL,0,&Func,arg,0,NULL);

    WaitForMultipleObjects(info.dwNumberOfProcessors,th,true,INFINITE);
   
    clock_t etime = clock();
    cout<<"main: "<< counter<<"time: "<<(double)(etime-stime)/CLOCKS_PER_SEC << endl;
    return 0;
}

unsigned __stdcall Func(LPVOID arg)
{
    int start = *((int*)arg);
    int end = *((int*)arg+1);
    for(int i = start; i < end; i++)
    {
        EnterCriticalSection(&cs);      //3. key 획득
        counter++;
        LeaveCriticalSection(&cs);      //4. key반납
    }
    return 0;
}