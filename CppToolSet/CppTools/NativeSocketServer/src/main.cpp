
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




	return 0;
}