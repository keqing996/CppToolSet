
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

    SOCKET socket = createSocketResult.result;

    // Bind socket
    auto bindResult = WindowsApi::Socket::Bind(&socket, ipWStr, port);
    if (!bindResult.success)
    {
        std::wcout << bindResult.errorMessage << std::endl;
        WindowsApi::Socket::CleanWinSocketsEnvironment();
        WindowsApi::Socket::CloseSocket(&socket);
        std::cin.get();
        return 1;
    }

    // Listen socket
    auto listenResult = WindowsApi::Socket::Listen(&socket);
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

    auto wsaEvent = eventCreatResult.result;

    auto selectResult = WindowsApi::Socket::EventSelect(&socket, wsaEvent, FD_ACCEPT | FD_CLOSE);
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
        auto index = WindowsApi::Socket::WaitForMultipleEvents(
                dwTotal, eventArray);

        auto fixedIndex = index - WSA_WAIT_EVENT_0;

        auto enumEventResult = WindowsApi::Socket::EnumEvents(
                &socketArray[fixedIndex], eventArray[fixedIndex]);

        if (!enumEventResult.success)
        {
            std::wcout << selectResult.errorMessage << std::endl;
            break;
        }

        if (WindowsApi::Socket::EnumEventsIsAccept(enumEventResult))
        {
            auto acceptResult = WindowsApi::Socket::Accept(&socketArray[fixedIndex]);
            if (!acceptResult.success)
            {
                std::wcout << acceptResult.errorMessage << std::endl;
            }
            else
            {
                auto acceptAddr = acceptResult.result;
                auto parseAddrResult = WindowsApi::Socket::GetIpv4FromAddr(acceptAddr);
                if (parseAddrResult.success)
                {
                    std::wcout << std::format(L"Accept {}: {}", parseAddrResult.result1, parseAddrResult.result2);
                }

            }
        }
        else if (WindowsApi::Socket::EnumEventsIsAccept(enumEventResult))
        {

        }
        else if (WindowsApi::Socket::EnumEventsIsAccept(enumEventResult))
        {

        }
        else if (WindowsApi::Socket::EnumEventsIsAccept(enumEventResult))
        {

        }
    }



	return 0;
}