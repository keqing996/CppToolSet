#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <string>
#include <vector>

namespace CppServer
{
    class TcpSocket
    {
    public:
        TcpSocket();
        ~TcpSocket();

    public:
        void Run();
        bool IsInit() const;

    private:
        std::vector<std::string> GetHostIp() const;

    private:
        bool _init;
        SOCKET _serverSocket;
    };
}

