
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>
#include <queue>
#include <format>

#include "cmdline/cmdline.h"
#include "WinApiSocket.h"
#include "StringUtil.hpp"

int main(int argc, char* argv[])
{
    CmdLine::Parser cmd;

    cmd.Add<std::string>("ip", 'i', "ip", true, "");
    cmd.Add<int>("port", 'p', "port number", true, 80, CmdLine::Range(1, 65535));

    cmd.ParseCheck(argc, argv);

    std::string ipStr = cmd.Get<std::string>("ip");
    std::wstring ipWStr = StringUtil::StringToWString(ipStr);

    int port = cmd.Get<int>("port");

    // Init WSA
    auto initResult = WindowsApi::Socket::InitWinSocketsEnvironment();
    if (!initResult.success)
    {
        std::wcout << initResult.errorMessage << std::endl;
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        std::cin.get();
        return 1;
    }

    // Create socket
    auto createSocketResult = WindowsApi::Socket::CreateTcpIpv4Socket();
    if (!createSocketResult.success)
    {
        std::wcout << initResult.errorMessage << std::endl;
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        std::cin.get();
        return 1;
    }

    SOCKET socket = createSocketResult.socket;

    // Bind socket
    auto bindResult = WindowsApi::Socket::SocketBind(&socket, ipWStr, port);
    if (!bindResult.success)
    {
        std::wcout << bindResult.errorMessage << std::endl;
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        WindowsApi::Socket::CloseSocket(&socket);
        std::cin.get();
        return 1;
    }

    // Listen socket
    auto listenResult = WindowsApi::Socket::SocketListen(&socket);
    if (!listenResult.success)
    {
        std::wcout << listenResult.errorMessage << std::endl;
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        WindowsApi::Socket::CloseSocket(&socket);
        std::cin.get();
        return 1;
    }

    // Create event
    auto eventCreatResult = WindowsApi::Socket::SocketCreateEvent();
    if (!eventCreatResult.success)
    {
        std::wcout << eventCreatResult.errorMessage << std::endl;
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        WindowsApi::Socket::CloseSocket(&socket);
        std::cin.get();
        return 1;
    }

    auto wsaEvent = eventCreatResult.event;

    auto selectResult = WindowsApi::Socket::SocketEventSelect(&socket, wsaEvent, FD_ACCEPT | FD_CLOSE);
    if (!selectResult.success)
    {
        std::wcout << selectResult.errorMessage << std::endl;
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        WindowsApi::Socket::CloseSocket(&socket);
        std::cin.get();
        return 1;
    }

    SOCKET socketArray[WSA_MAXIMUM_WAIT_EVENTS];
    WSAEVENT eventArray[WSA_MAXIMUM_WAIT_EVENTS];
    DWORD dwTotal = 0;

    socketArray[dwTotal] = socket;
    eventArray[dwTotal] = wsaEvent;
    dwTotal++;

    for (;;)
    {
        auto index = WindowsApi::Socket::SocketWaitForMultipleEvents(
                dwTotal, eventArray);

        auto fixedIndex = index - WSA_WAIT_EVENT_0;

        auto enumEventResult = WindowsApi::Socket::SocketEnumNetworkEvents(
                &socketArray[fixedIndex], eventArray[fixedIndex]);

        if (!enumEventResult.success)
        {
            std::wcout << selectResult.errorMessage << std::endl;
            break;
        }

        if (enumEventResult.IsAccept())
        {
            auto acceptResult = WindowsApi::Socket::SocketAccept(&socketArray[fixedIndex]);
            if (!acceptResult.success)
            {
                std::wcout << acceptResult.errorMessage << std::endl;
            }
            else
            {
                auto acceptAddr = acceptResult.acceptAddr;
                std::wcout << std::format(L"Accept {}: {}", ::inet_ntoa(acceptAddr.sin_addr), ::ntohs(acceptAddr.sin_port));
            }
        }
        else if (enumEventResult.IsRead())
        {

        }
        else if (enumEventResult.IsClose())
        {

        }
        else if (enumEventResult.IsWrite())
        {

        }
    }



	return 0;
}