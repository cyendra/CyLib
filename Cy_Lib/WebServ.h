#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <WinSock2.h>
#include <process.h>

namespace WebServ {
	const int BUF_SIZE = 2048;
	const int BUF_SMALL = 100;
	unsigned WINAPI RequestHandler(void* arg);
	char* ContentType(char* file);
	void SendData(SOCKET sock, char* ct, char* fileNmae);
	void SendErrorMSG(SOCKET sock);
	void ErrorHandling(char* message);

	int Start(char hostport[]) {
		WSADATA wsaData;
		SOCKET hServSock, hClntSock;
		SOCKADDR_IN servAdr, clntAdr;

		HANDLE hThread;
		DWORD dwThreadID;
		int clntAdrSize;

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup error");
		hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		memset(&servAdr, 0, sizeof(servAdr));
		servAdr.sin_family = AF_INET;
		servAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		servAdr.sin_port = htons(atoi(hostport));

		if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) ErrorHandling("bind error");
		if (listen(hServSock, 5) == SOCKET_ERROR) ErrorHandling("listen error");

		for (;;) {
			clntAdrSize = sizeof(clntAdr);
			hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
			printf("Connection Request : %s:%d\n", inet_ntoa(clntAdr.sin_addr), ntohs(clntAdr.sin_port));
			hThread = (HANDLE)_beginthreadex(NULL, 0, RequestHandler, (void*)hClntSock, 0, (unsigned*)&dwThreadID);
		}
		closesocket(hServSock);
		WSACleanup();
		return 0;
	}

	unsigned WINAPI RequestHandler(void *arg) {
		SOCKET hClntSock = (SOCKET)arg;
		char buf[BUF_SIZE];
		char method[BUF_SMALL];
		char ct[BUF_SMALL];
		char fileName[BUF_SMALL];

		recv(hClntSock, buf, BUF_SIZE, 0);
		if (strstr(buf, "HTTP/") == NULL) {
			SendErrorMSG(hClntSock);
			closesocket(hClntSock);
			return 1;
		}

		strcpy(method, strtok(buf, " /"));
		if (strcmp(method, "GET")) SendErrorMSG(hClntSock);

		strcpy(fileName, strtok(NULL, " /"));
		strcpy(ct, ContentType(fileName));
		SendData(hClntSock, ct, fileName);
		return 0;
	}

	void SendData(SOCKET sock, char* ct, char* fileName) {
		char protocol[] = "HTTP/1.0 200 OK\r\n";
		char servName[] = "Server:simple web server\r\n";
		char cntLen[] = "Content-length:2048\r\n";
		char cntType[BUF_SMALL];
		char buf[BUF_SIZE];
		FILE* sendFile;

		sprintf(cntType, "Content-type:%s\r\n\r\n", ct);
		if ((sendFile = fopen(fileName, "r")) == NULL) {
			SendErrorMSG(sock);
			return;
		}

		send(sock, protocol, strlen(protocol), 0);
		send(sock, servName, strlen(servName), 0);
		send(sock, cntLen, strlen(cntLen), 0);
		send(sock, cntType, strlen(cntType), 0);

		while (fgets(buf, BUF_SIZE, sendFile) != NULL) send(sock, buf, strlen(buf), 0);

		closesocket(sock);
	}

	void SendErrorMSG(SOCKET sock) {
		char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
		char servName[] = "Server:simple web server\r\n";
		char cntLen[] = "Content-length:2048\r\n";
		char cntType[] = "Content-type:text/html\r\n\r\n";
		char content[] = "<html><head><title>NETWORK</title></head>"
			"<body><font size=+5><br> 发生错误！查看请求文件名和请求方式！"
			"</font></body></html>";
		send(sock, protocol, strlen(protocol), 0);
		send(sock, servName, strlen(servName), 0);
		send(sock, cntLen, strlen(cntLen), 0);
		send(sock, cntType, strlen(cntType), 0);
		send(sock, content, strlen(content), 0);
		closesocket(sock);
	}

	char* ContentType(char* file) {
		char extension[BUF_SMALL];
		char fileName[BUF_SMALL];
		strcpy(fileName, file);
		strtok(fileName, ".");
		strcpy(extension, strtok(NULL, "."));
		if (!strcmp(extension, "html") || !strcmp(extension, "htm")) return "text/html";
		else return "text/plain";
	}

	void ErrorHandling(char* message) {
		fputs(message, stderr);
		fputc('\n', stderr);
		exit(1);
	}

}