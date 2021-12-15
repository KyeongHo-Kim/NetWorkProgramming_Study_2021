#pragma once
#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<vector>

using std::endl;
using std::cout;
using std::vector;
using std::find;

typedef struct session Session;
typedef struct server Server;

enum class IOtype						 // Clean_Session()함수에서 송 수신을 판단 하기 위함
{
	Read, Write
};

typedef struct session
{
	WSAOVERLAPPED over;					// WSArecv, WSAsend, WSAGetOverlappedResult에서 사용됨 

	WSABUF rwsa, swsa;					// Scatter_Gather를 위한 연결 버퍼
	char rbuf[80], sbuf[80];			// 실제 데이터 송·수신 버퍼

	SOCKET tcp_sock;
	WSAEVENT event;

	void (*handle)(Session*, DWORD);	// 세션정보를 처리 즉 write_process read_process를 처리할때 사용

	Server* server;						// 서버를 가르키는 포인터
};

typedef struct server
{
	SOCKET gate_sock;

	vector<Session*> sarr;				// 세션이 생성되면 추가함
	vector<WSAEVENT> earr;				// WSAWaitForMultipleEvents에서 활용하기 위함
	int noc;							// 체크해야할 이벤트 객체의 갯수

	CRITICAL_SECTION cs;
};


// winsock 초기화 부터 bind까지 진행 후 서버정보 초기화(소켓, noc citicalsection);
bool Server_Init(Server*);

unsigned __stdcall ProActor(LPVOID);

// 세션 생성 및 제거 함수
Session* Add_Session(Server*, SOCKET);
void Close_Session(Session*);

// 에코 서비스 함수
void Read_Process(Session*, DWORD);
void Write_Process(Session*, DWORD);

// 패킷 송˙수신 동작을 하는 함수
void Clean_Session(Session*, int, IOtype);
void Packet_Recv(Session*);
void Packet_Send(Session*, int);

void Err_Display(const char*);		   
