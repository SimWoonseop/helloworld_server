#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void ErrorHendling(const char* message);

int main()
{
	WSADATA wasData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	short port = 5001;
	int szClntAddr;
	char message[] = "HelloWorld!";


	if (WSAStartup(MAKEWORD(2, 2), &wasData) != 0)
	{
		ErrorHendling("WSAStartup() error!");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	if (hServSock == INVALID_SOCKET)
	{
		ErrorHendling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHendling("bind() error!");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHendling("listen() error!");
	}

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);

	if (hClntSock == INVALID_SOCKET)
	{
		ErrorHendling("accept() error!");
	}

	send(hClntSock, message, sizeof(message), 0);

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHendling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}