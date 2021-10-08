/*
    동작과정

    mutexcreate()가 signaled 상태로 변경되면 WaitForSingleObject를 빠져나와 key를 획득하게 된다.
    counter를 증가시키고 끝나면
    eleasemutexr()를 통해 mutex의 상태를 다시 signal상태로 만든다.
    해당과정을 반복한다.

    auto_reset = WaitForSingleObject를 빠져 나오면서 signaled => non_signalde 상태로 만드는것
    manual_reset = WaitForSingleObject를 빠져 나오면서 signaled상태를 유지하는것
    non-signal로 변경할려면 함수를 사용해야 한다.

*/

#include<iostream>
#include<WinSock2.h>
#include<process.h>

using std::endl;
using std::cout;


unsigned __stdcall Func(LPVOID);

int counter;

int main()
{
    int arg[] = {0,10000};
    HANDLE th[20];

    
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
        counter++;
    }
    return 0;
}
