#pragma once

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef UNICODE
#define UNICODE
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <string>
#include <thread>
#include <WinSock2.h>

namespace WindowsApi::Socket
{
    using Byte = char;

    struct ActionResult
    {
        bool success;
        std::wstring errorMessage;
    };

    struct SocketCreateResult
    {
        bool success;
        SOCKET socket;
        std::wstring errorMessage;
    };

    struct SocketReceiveResult
    {
        bool success;
        int receiveSize;
        std::wstring errorMessage;
    };

    struct SocketAcceptResult
    {
        bool success;
        sockaddr_in acceptAddr;
        std::wstring errorMessage;
    };

    struct SocketCreateEventResult
    {
        bool success;
        void* event;
        std::wstring errorMessage;
    };

    ActionResult InitWinSocketsEnvironment();

    void CleanWinSocketsEnvironment();

    SocketCreateResult CreateTcpIpv4Socket();

    void CloseSocket(const SOCKET* pSocket);

    ActionResult SocketSend(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize);

    SocketReceiveResult SocketReceive(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize);

    ActionResult SocketConnect(const SOCKET* pSocket, std::wstring ipStr, int port);

    ActionResult SocketBind(const SOCKET* pSocket, std::wstring ipStr, int port);

    ActionResult SocketListen(const SOCKET* pSocket);

    SocketAcceptResult SocketAccept(const SOCKET* pSocket);



}