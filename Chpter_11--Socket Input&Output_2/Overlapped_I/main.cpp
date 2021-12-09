#pragma comment(lib,"ws2_32.lib")
#include "Over1.hpp";
#include<process.h>

int main()
{
	Server server;

	if (Server_Init(&server) == false)
	{
		cout << "Server_Init_error" << endl;
		return -1;
	}

	_beginthreadex(NULL, 0, ProActor, &server, 0, NULL);

	if (listen(server.gate_sock,SOMAXCONN))
	{
		Err_Display("listen()");
		return -1;
	}

	SOCKADDR_IN caddr;
	int caddrlen = sizeof(caddr);
	SOCKET c_sock;

	Session* ses = NULL;
	while (true)
	{
		c_sock = accept(server.gate_sock, (SOCKADDR*)&caddr, &caddrlen);
		if (c_sock == INVALID_SOCKET)
		{
			Err_Display("accept()");
			break;
		}

		// session 생성
		ses = Add_Session(&server, c_sock);

		//packet 수신
		Packet_Recv(ses);

	}

	closesocket(server.gate_sock);
	return 0;
}