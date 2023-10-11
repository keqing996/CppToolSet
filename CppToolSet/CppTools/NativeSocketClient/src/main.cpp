
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <queue>

#include "cmdline/cmdline.h"
#include "WinApiSocket.h"
#include "WinApiConsole.h"
#include "StringUtil.hpp"

HANDLE hConsoleHandle;
int SEND_BUFFER_SIZE = 1024;
int RECEIVE_BUFFER_SIZE = 4096;

void SetConsoleColor(WindowsApi::Console::ConsoleColor c)
{
    WindowsApi::Console::SetColor(hConsoleHandle, c, WindowsApi::Console::ConsoleColor::Black);
}

void LogError(const std::string& message)
{
    SetConsoleColor(WindowsApi::Console::ConsoleColor::Red);
    std::cout << message << std::endl;
    SetConsoleColor(WindowsApi::Console::ConsoleColor::None);
}

void LogError(const std::wstring& message)
{
    SetConsoleColor(WindowsApi::Console::ConsoleColor::Red);
    std::wcout << message << std::endl;
    SetConsoleColor(WindowsApi::Console::ConsoleColor::None);
}

int main(int argc, char* argv[])
{
    CmdLine::Parser cmd;

    cmd.Add<std::string>("ip", 'i', "ip", true, "");
    cmd.Add<int>("port", 'p', "port number", true, 80, CmdLine::Range(1, 65535));

    cmd.ParseCheck(argc, argv);

    std::string ipStr = cmd.Get<std::string>("ip");
    std::wstring ipWStr = StringUtil::StringToWString(ipStr);

    int port = cmd.Get<int>("port");

    hConsoleHandle = WindowsApi::Console::GetStdOutputHandle();

    // Init WSA
    auto initResult = WindowsApi::Socket::InitWinSocketsEnvironment();
    if (!initResult.success)
    {
        LogError(initResult.errorMessage);
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        std::cin.get();
        return 1;
    }

    // Create socket
    auto createSocketResult = WindowsApi::Socket::CreateTcpIpv4Socket();
    if (!createSocketResult.success)
    {
        LogError(createSocketResult.errorMessage);
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        std::cin.get();
        return 1;
    }

    SOCKET socket = createSocketResult.result;

    // Connect socket
    auto connectResult = WindowsApi::Socket::Connect(&socket, ipWStr, port);
    if (!connectResult.success)
    {
        LogError(connectResult.errorMessage);
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        WindowsApi::Socket::CloseSocket(&socket);
        std::cin.get();
        return 1;
    }

    std::shared_ptr<char> pSendBuffer {new WindowsApi::Socket::Byte[SEND_BUFFER_SIZE]};
    std::shared_ptr<char> pReceiveBuffer {new WindowsApi::Socket::Byte[RECEIVE_BUFFER_SIZE]};
    std::atomic<bool> shouldStop = false;
    std::mutex threadMt;

    // Receive thread
    std::thread receiveThread([&threadMt, pReceiveBuffer, socket, &shouldStop]() -> void
    {
        while (true)
        {
            if (shouldStop.load())
                break;

            ::ZeroMemory(pReceiveBuffer.get(), RECEIVE_BUFFER_SIZE);

            auto receiveResult = WindowsApi::Socket::Receive(&socket, pReceiveBuffer.get(), RECEIVE_BUFFER_SIZE);
            if (!receiveResult.success)
            {
                std::lock_guard lockGuard(threadMt);
                LogError(receiveResult.errorMessage);
                shouldStop.store(false);
                break;
            }

            if (receiveResult.result > 0)
            {
                std::string_view receiveData(pReceiveBuffer.get());
                std::lock_guard lockGuard(threadMt);

                SetConsoleColor(WindowsApi::Console::ConsoleColor::Green);
                std::cout << "\nReceive:";
                SetConsoleColor(WindowsApi::Console::ConsoleColor::None);
                std::cout << receiveData << "\n" << std::endl;

                SetConsoleColor(WindowsApi::Console::ConsoleColor::Cyan);
                std::cout << "\nSend:";
                SetConsoleColor(WindowsApi::Console::ConsoleColor::None);
            }

            Sleep(1000);
        }
    });

    // Send thread
	while (true)
	{
        if (shouldStop.load())
            break;

        ::ZeroMemory(pSendBuffer.get(), SEND_BUFFER_SIZE);

        std::cin.getline(pSendBuffer.get(), SEND_BUFFER_SIZE - 2); // final zero will change to \n

        std::string_view sendStr {pSendBuffer.get()};

        (pSendBuffer.get())[sendStr.length()] = '\r';
        (pSendBuffer.get())[sendStr.length() + 1] = '\n';

        auto sendResult = WindowsApi::Socket::Send(&socket, pSendBuffer.get(), sendStr.length() + 2);
        if (!sendResult.success)
        {
            std::lock_guard lockGuard {threadMt};
            LogError(sendResult.errorMessage);
            shouldStop.store(false);
            break;
        }
	}

    receiveThread.join();

    std::cout << "socket end" << std::endl;

    WindowsApi::Socket::CloseSocket(&socket);
    WindowsApi::Socket::CleanWinSocketsEnvironment();

    std::cin.get();
	return 0;
}