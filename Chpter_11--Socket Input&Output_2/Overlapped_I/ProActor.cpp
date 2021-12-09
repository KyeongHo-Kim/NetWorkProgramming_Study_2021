#include "Over1.hpp"

unsigned __stdcall ProActor(LPVOID arg)
{
	Server* ser = (Server*)arg;	//Server의 포인터를 얻어옴

	int ret1, ret2;
	Session* ses = NULL;
	DWORD trans;
	DWORD flag;
	while (true)
	{
		// 감시할 이벤트 객체가 1개 이상 되어야 WSAWaitForMultipleEvents()함수가 정상 동작한다.
		if (ser->noc > 0)
		{
			ret1 = WSAWaitForMultipleEvents(ser->noc, &ser->earr[0], false, 200, false) - WSA_WAIT_EVENT_0;
			if (ret1 == WSA_WAIT_FAILED || ret1 == WSA_WAIT_TIMEOUT)
			{
				Sleep(200);
				continue;
			}

		
		
		// 동시에 signal상태로 바뀐 이벤트 객체를 체크한다.
			for (int idx = ret1; idx < ser->noc; idx++)
			{
				ret2 = WSAWaitForMultipleEvents(1, &ser->earr[idx], true, 0, false);
				if (ret2 == WSA_WAIT_FAILED || ret2 == WSA_WAIT_TIMEOUT)
				{
					continue;
				}

				// signal상태로 변경된 소켓을 세션에 저장한다.
				ses = ser->sarr[idx];

				// WSAGetOverlappedResult함수가 false일때 
				if (!WSAGetOverlappedResult(ses->tcp_sock, &ses->over, &trans, true, &flag))
				{
					if (WSAGetLastError() == WSAECONNRESET)		// 연결이 강제로 종료되었을 경우
					{
						cout << "abnomal close case" << endl;
					}
					else
						Err_Display("WSAGetOverlappedResult()");

					Close_Session(ses);
					continue;
				}

				// WSAGetOverlappedResult함수가 true일때
				// 세션정보와 size를 넘긴다.
				ses->handle(ses, trans);	// Close_Session에서 설정한 Read/Write_Process함수 호출
			}
		}
	}
	
	return 0;
}