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
    for(int i = 0; i < info.dwNumberOfProcessors; i++)
        th[i] = (HANDLE)_beginthreadex(NULL,0,&Func,arg,0,NULL);

    WaitForMultipleObjects(info.dwNumberOfProcessors,th,true,INFINITE);
   

    cout<<counter<<endl;
    return 0;
}

unsigned __stdcall Func(LPVOID arg)
{
    int start = *((int*)arg);
    int end = *((int*)arg+1);
    for(int i = start; i < end; i++)
        counter++;

    return 0;
}