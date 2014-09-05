#include <WinSock2.h>
#include <iostream>

namespace WinSock {

	void ErrorHandling(char* message) {
		std::cerr << message << std::endl;
		system("pause");
		exit(1);
	}

	int Server(int port) {
		WSADATA wsaData;
		SOCKET hServSock, hClntSock;
		SOCKADDR_IN servAddr, clntAddr;
		int szClntAddr;
		char message[] = "Hello World!";
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error");
		hServSock = socket(PF_INET, SOCK_STREAM, 0);
		if (hServSock == INVALID_SOCKET) ErrorHandling("socket() error");
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(port);
		if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) ErrorHandling("bind() error");
		if (listen(hServSock, 5) == SOCKET_ERROR) ErrorHandling("listen() error");
		szClntAddr = sizeof(clntAddr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
		if (hClntSock == INVALID_SOCKET) ErrorHandling("accept() error");
		send(hClntSock, message, sizeof(message), 0);
		closesocket(hClntSock);
		closesocket(hServSock);
		WSACleanup();
		return 0;
	}

	int Client(char ip[], int port) {
		WSADATA wsaData;
		SOCKET hSocket;
		SOCKADDR_IN servAddr;
		char message[30];
		int strLen;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error");
		hSocket = socket(PF_INET, SOCK_STREAM, 0);
		if (hSocket == INVALID_SOCKET) ErrorHandling("socket() error");
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.S_un.S_addr = inet_addr(ip);
		servAddr.sin_port = htons(port);
		if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) ErrorHandling("connect() error");
		strLen = recv(hSocket, message, sizeof(message) - 1, 0);
		if (strLen == -1) ErrorHandling("read() error");
		std::cout << "Message from server: " << message << std::endl;
		closesocket(hSocket);
		WSACleanup();
		return 0;
	}

	void GetHostByName(const char name[]) {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error");
		hostent* host;
		host = gethostbyname(name);
		if (!host) ErrorHandling("gethost error");
		std::cout << "Official name: " << host->h_name << std::endl;
		for (int i = 0; host->h_aliases[i]; i++) {
			std::cout << "Aliases " << i + 1 << ": " << host->h_aliases[i] << std::endl;
		}
		std::cout << "Address type: " << (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6") << std::endl;
		for (int i = 0; host->h_addr_list[i]; i++) {
			std::cout << "IP addr " << i + 1 << ": " << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << std::endl;
		}
		WSACleanup();
	}

	void GetHostByAddr(const char ip[]) {
		WSADATA wsaData;
		hostent* host;
		SOCKADDR_IN addr;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error");
		memset(&addr, 0, sizeof(addr));
		addr.sin_addr.S_un.S_addr = inet_addr(ip);
		host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
		if (!host) ErrorHandling("gethost error");
		std::cout << "Official name: " << host->h_name << std::endl;
		for (int i = 0; host->h_aliases[i]; i++) {
			std::cout << "Aliases " << i + 1 << ": " << host->h_aliases[i] << std::endl;
		}
		std::cout << "Address type: " << (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6") << std::endl;
		for (int i = 0; host->h_addr_list[i]; i++) {
			std::cout << "IP addr " << i + 1 << ": " << inet_ntoa(*(in_addr*)host->h_addr_list[i]) << std::endl;
		}
		WSACleanup();
	}

};