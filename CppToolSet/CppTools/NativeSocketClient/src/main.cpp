
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

#include "cmdline/cmdline.h"
#include "WinApiSocket.h"
#include "StringUtil.hpp"

WindowsApi::SocketClient* pSocketClient;
std::atomic<bool> shouldStop = false;

int SEND_BUFFER_SIZE = 1024;
int RECEIVE_BUFFER_SIZE = 4096;

int main(int argc, char* argv[])
{
    CmdLine::Parser cmd;

    cmd.Add<std::string>("ip", 'i', "ip", true, "");
    cmd.Add<int>("port", 'p', "port number", true, 80, CmdLine::Range(1, 65535));

    cmd.ParseCheck(argc, argv);

    std::string ipStr = cmd.Get<std::string>("ip");
    std::wstring ipWStr = StringUtil::StringToWString(ipStr);
    int port = cmd.Get<int>("port");

    pSocketClient = new WindowsApi::SocketClient{};

    auto initResult = pSocketClient->InitSocket(WindowsApi::SocketMode::IPv4Tcp);
    if (!initResult.first)
    {
        std::wcout << initResult.second << std::endl;
        system("pause");
        delete pSocketClient;
        return 1;
    }

    auto connectResult = pSocketClient->Connect(ipWStr, port);
    if (!connectResult.first)
    {
        std::wcout << connectResult.second << std::endl;
        system("pause");
        delete pSocketClient;
        return 1;
    }

    char* sendBuffer = new char[SEND_BUFFER_SIZE];
    char* receiveBuffer = new char[RECEIVE_BUFFER_SIZE];

    std::thread receiveThread([receiveBuffer]() -> void
    {
        while (true)
        {
            if (shouldStop.load())
                break;

            ::ZeroMemory(receiveBuffer, RECEIVE_BUFFER_SIZE);

            int receiveSize = -1;
            auto receiveResult = pSocketClient->Receive(receiveBuffer, RECEIVE_BUFFER_SIZE, &receiveSize);
            if (!receiveResult.first)
            {
                std::wcout << receiveResult.second << std::endl;
                shouldStop.store(false);
                break;
            }

            if (receiveSize > 0)
            {
                std::string_view receiveData(receiveBuffer);
                std::cout << receiveData << std::endl;
            }

            Sleep(1000);
        }
    });

	while (true)
	{
        if (shouldStop.load())
            break;

        ::ZeroMemory(sendBuffer, SEND_BUFFER_SIZE);

        std::cin.getline(sendBuffer, SEND_BUFFER_SIZE - 2); // final zero will change to \n

        std::string_view sendStr {sendBuffer};

        sendBuffer[sendStr.length()] = '\r';
        sendBuffer[sendStr.length() + 1] = '\n';

        auto sendResult = pSocketClient->Send(sendBuffer, sendStr.length() + 2);
        if (!sendResult.first)
        {
            std::wcout << sendResult.second << std::endl;
            shouldStop.store(false);
            break;
        }
	}

    receiveThread.join();
    std::cout << "socket end" << std::endl;
    system("pause");
	return 0;
}