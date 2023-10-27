
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

void SetConsoleColor(WinApi::Console::ConsoleColor c)
{
    WinApi::Console::SetColor(hConsoleHandle, c, WinApi::Console::ConsoleColor::Black);
}

void LogError(const std::string& message)
{
    SetConsoleColor(WinApi::Console::ConsoleColor::Red);
    std::cout << message << std::endl;
    SetConsoleColor(WinApi::Console::ConsoleColor::None);
}

void LogError(const std::wstring& message)
{
    SetConsoleColor(WinApi::Console::ConsoleColor::Red);
    std::wcout << message << std::endl;
    SetConsoleColor(WinApi::Console::ConsoleColor::None);
}

int main(int argc, char* argv[])
{
    CmdLine::Parser cmd;

    cmd.Add<std::string>("ip", 'i', "ip", true, "");
    cmd.Add<int>("port", 'p', "port number", true, 80, CmdLine::Range(1, 65535));

    cmd.ParseCheck(argc, argv);

    std::string ipStr = cmd.Get<std::string>("ip");
    int port = cmd.Get<int>("port");

    WinApi::Console::SetConsoleOutputUtf8();
    hConsoleHandle = WinApi::Console::GetStdOutputHandle();

    // Init WSA
    if (!WinApi::Socket::StartUp())
    {
        LogError(WinApi::Socket::LastErrorMessage());
        WinApi::Socket::CleanUp();
        std::cin.get();
        return 1;
    }

    // Create socket
    auto createSocketResult = WinApi::Socket::CreateIpv4TcpSocket();
    if (!createSocketResult.has_value())
    {
        LogError(WinApi::Socket::LastErrorMessage());
        WinApi::Socket::CleanUp();
        std::cin.get();
        return 1;
    }

    WinApi::Socket::SocketHandle socket = createSocketResult.value();

    // Connect socket
    if (!WinApi::Socket::Connect(&socket, ipStr, port))
    {
        LogError(WinApi::Socket::LastErrorMessage());
        WinApi::Socket::CloseSocket(socket);
        WinApi::Socket::CleanUp();
        std::cin.get();
        return 1;
    }

    std::shared_ptr<char> pSendBuffer {new WinApi::Socket::Byte[SEND_BUFFER_SIZE]};
    std::shared_ptr<char> pReceiveBuffer {new WinApi::Socket::Byte[RECEIVE_BUFFER_SIZE]};
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

            auto receiveResult = WinApi::Socket::Receive(socket, pReceiveBuffer.get(), RECEIVE_BUFFER_SIZE);
            if (!receiveResult.has_value())
            {
                std::lock_guard lockGuard(threadMt);
                LogError(WinApi::Socket::LastErrorMessage());
                shouldStop.store(false);
                break;
            }

            if (receiveResult.value() > 0)
            {
                std::string_view receiveData(pReceiveBuffer.get());
                std::lock_guard lockGuard(threadMt);

                SetConsoleColor(WinApi::Console::ConsoleColor::Green);
                std::cout << "\nReceive:";
                SetConsoleColor(WinApi::Console::ConsoleColor::None);
                std::cout << receiveData << "\n" << std::endl;

                SetConsoleColor(WinApi::Console::ConsoleColor::Cyan);
                std::cout << "\nSend:";
                SetConsoleColor(WinApi::Console::ConsoleColor::None);
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

        if (!WinApi::Socket::Send(socket, pSendBuffer.get(), sendStr.length() + 2))
        {
            std::lock_guard lockGuard {threadMt};
            LogError(WinApi::Socket::LastErrorMessage());
            shouldStop.store(false);
            break;
        }
	}

    receiveThread.join();

    std::cout << "socket end" << std::endl;

    WinApi::Socket::CloseSocket(socket);
    WinApi::Socket::CleanUp();

    std::cin.get();
	return 0;
}