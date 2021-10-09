/*
        동작과정
event 핸들값을 저장할 전역 변수를 만듬

event kernel object를 생성하고 manul reset nonsignald 상태로 설정
nonsignald 상태이기 때문에 WaitForSingObject에 의해 막힘
SetEvent함수를 사용하여 non-signald -> signal상태로 변경 후
ResEvent 함수를 만나 signal -> non-signald 상태로 변환됨

작업을 다끝 내면 

SetEvent함수를 만다 
non-signald -> signal 변환
*/

#include<iostream>
#include<WinSock2.h>
#include<process.h>

using std::endl;
using std::cout;

HANDLE hevent;                     // kernel object 저장 handle

unsigned __stdcall Func(LPVOID);

int counter;

int main()
{
    int arg[] = { 0,10000 };
    HANDLE th[20];

    hevent = CreateEvent(NULL, true, false, NULL);        //1. Event kernel object를 생성한다.       현재 manul reset / non_signaled 상태

        SYSTEM_INFO info;
    GetSystemInfo(&info);

    clock_t stime = clock();

    for (int i = 0; i < info.dwNumberOfProcessors; i++)
        th[i] = (HANDLE)_beginthreadex(NULL, 0, &Func, arg, 0, NULL);

    SetEvent(hevent);                                                           // 현재 non_signaled -> signaled상태로 변환

    WaitForMultipleObjects(info.dwNumberOfProcessors, th, true, INFINITE);

    clock_t etime = clock();
    cout << "main: " << counter << "  " << "time: " << (double)(etime - stime) / CLOCKS_PER_SEC << endl;
    return 0;
}

unsigned __stdcall Func(LPVOID arg)
{
    int start = *((int *)arg);
    int end = *((int *)arg + 1);
    for (int i = start; i < end; i++)
    {
        WaitForSingleObject(hevent, INFINITE);        //  manul reset 현재 signaled 상태
        ResetEvent(hevent);                           //  signalednon_signaled 변환
        counter++;
        SetEvent(hevent);                             //  signaled 상태로 변환
    }
    return 0;
}



