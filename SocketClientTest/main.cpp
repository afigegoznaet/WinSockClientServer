#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class SocketClient {
public:
	SocketClient() {
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		//hints.ai_flags = AI_PASSIVE;


		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		assert(0 == iResult);

		iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
		assert(0 == iResult);

		ConnectSocket = socket(result->ai_family, result->ai_socktype,
							   result->ai_protocol);
		assert(INVALID_SOCKET != ConnectSocket);

		// Connect to server.
		iResult = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
		assert(SOCKET_ERROR != iResult);
	}
	int receiveData() {
		auto res = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		printf("Bytes received: %d\n", res);
		return res;
	}

	int sendData() {
		auto iSendResult = send(ConnectSocket, recvbuf, recvbuflen, 0);
		printf("Bytes senr: %d\n", iSendResult);
		return iSendResult;
	}

	~SocketClient() {
		freeaddrinfo(result);
		closesocket(ConnectSocket);
		WSACleanup();
	}

private:
	SOCKET			 ConnectSocket{INVALID_SOCKET};
	struct addrinfo	 hints {};
	struct addrinfo *result{};
	WSADATA			 wsaData{};

	char recvbuf[DEFAULT_BUFLEN] = "Hello hello";
	int	 recvbuflen = DEFAULT_BUFLEN;
};

int __cdecl main() {

	SocketClient client;
	client.sendData();
	client.receiveData();
	return 0;
}
