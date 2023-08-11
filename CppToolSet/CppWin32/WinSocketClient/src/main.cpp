
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (0 != WSAStartup(wVersion, &wsadata))
	{
		std::cout << "socket init failed" << std::endl;
		system("pause");
		return 1;
	}

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
	{
		std::cout << "socket version failed" << std::endl;
		WSACleanup();
		system("pause");
		return 1;
	}

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	PCWSTR src = TEXT("127.0.0.1");
	in_addr dst;
	InetPton(AF_INET, src, &dst);
    
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = dst.S_un.S_addr;
	server_addr.sin_port = htons(4869);

	if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		std::cout << "socket connect failed:" << WSAGetLastError() << std::endl;
		WSACleanup();
		system("pause");
		return 1;
	}

	while (true)
	{
		char szSendBuffer[1500];

		std::string src = "hello";
		memcpy(szSendBuffer, src.c_str(), strlen(src.c_str()));

		if (SOCKET_ERROR == send(clientSocket, szSendBuffer, strlen(szSendBuffer) + 1, 0))
		{
			printf("send error :%d\n", WSAGetLastError());
			break;
		}

		Sleep(2000);
	}

	return 0;
}