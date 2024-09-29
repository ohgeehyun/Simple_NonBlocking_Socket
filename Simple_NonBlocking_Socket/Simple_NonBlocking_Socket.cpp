#pragma once
#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <iostream>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

void HandleError(const char* cause)
{
	int32_t errCode = ::WSAGetLastError();
	cout << "ErrorCode : " << errCode << endl;
}

int main()
{

	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "start up 에러" << endl;
		return 0;
	}
	//블로킹(blocking) 소켓
	//accpet -> 접속한 클라가있을때
	//connect -> 서버 접속 성공했을 때
	//send,sendto -> 요청한 데이터를 송신 버퍼에 복사했을떄.
	//recv,recvfrom-> 수신 버퍼에 도착한 데이터가있고, 이를 유저레벨 버퍼에 복사했을 떄

	//논블로킹(non-blocking)
	SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(listenSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(5252);

	if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		return 0;

	if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		return 0;

	cout << "Accept" << endl;

	SOCKADDR_IN clientAddr;
	int32_t addrLen = sizeof(clientAddr);

	// Accept
	while (true)
	{
		SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSocket == INVALID_SOCKET)
		{
			//원래 block을하면 걸려야하지만 논블로킹이기때문에 accpet이 제대로 되지않아도 통과해버리는 경우 존재
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			//Error
			break;
		}
		cout << "Client Connected!" << endl;



		//Recv
		while (true)
		{
			char recvBuffer[1000];
			int32_t recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen == SOCKET_ERROR)
			{
				//원래 블록했어야하지만 논블로킹으로인해 통과가 된경우
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				//error
				break;
			}
			else if (recvLen == 0)
			{
				//연결 끊김
				break;
			}

			cout << "Recv Data Len = " << recvLen << endl;

			//send
			while (true)
			{
				if (::send(clientSocket, recvBuffer, recvLen, 0) == SOCKET_ERROR)
				{
					//원래 블록이 되어야하지만 논블로킹하라며?
					if (::WSAGetLastError() == WSAEWOULDBLOCK)
						continue;

					//Error
					break;
				}

				cout << "Send Data ! Len = " << recvLen << endl;
				break;
			}
		}
	}

	//윈속 종료
	::WSACleanup();

}
