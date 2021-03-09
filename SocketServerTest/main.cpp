#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>


// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class SocketServer {
public:
	SocketServer() {
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;


		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		assert(0 == iResult);


		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		assert(0 == iResult);

		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		assert(ListenSocket != INVALID_SOCKET);

		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		assert(SOCKET_ERROR != iResult);
		iResult = listen(ListenSocket, SOMAXCONN);
		assert(SOCKET_ERROR != iResult);

		ClientSocket = accept(ListenSocket, NULL, NULL);
		assert(INVALID_SOCKET != ClientSocket);
	}

	int receiveData() {
		auto res = recv(ClientSocket, recvbuf, recvbuflen, 0);
		printf("Bytes received: %d\n", res);
		return res;
	}

	int sendData() {
		auto iSendResult = send(ClientSocket, recvbuf, recvbuflen, 0);
		printf("Bytes senr: %d\n", iSendResult);
		return iSendResult;
	}

	~SocketServer() {
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
	}

private:
	SOCKET			 ListenSocket{INVALID_SOCKET};
	SOCKET			 ClientSocket{INVALID_SOCKET};
	struct addrinfo	 hints {};
	struct addrinfo *result{};
	WSADATA			 wsaData{};

	char recvbuf[DEFAULT_BUFLEN];
	int	 recvbuflen = DEFAULT_BUFLEN;
};

int main() {
	SocketServer test;
	test.receiveData();
	test.sendData();


	return 0;
}
