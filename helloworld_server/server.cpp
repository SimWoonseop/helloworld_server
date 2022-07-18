#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHendling(const char* message);

int main()
{
	WSADATA wasData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;


	short port = 5001;
	int szClntAddr;
	int strLen;
	char message[BUF_SIZE];


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
	
	for (int i = 0; i < 5; i++)
	{
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);

		if (hClntSock == INVALID_SOCKET)
		{
			ErrorHendling("accept() error!");
		}
		else
		{
			printf("Connected Client %d \n", i + 1);
		}

		while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
		{
			send(hClntSock, message, strLen, 0);
		}

		closesocket(hClntSock);
	}

	
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