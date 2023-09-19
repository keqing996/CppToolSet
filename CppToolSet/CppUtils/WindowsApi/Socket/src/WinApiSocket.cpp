
#include "../include/WinApiSocket.h"

#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <format>

#pragma comment(lib, "Ws2_32.lib")

namespace WindowsApi::Socket
{
    ActionResult InitWinSocketsEnvironment()
    {
        WORD wVersion = MAKEWORD(2, 2);
        WSADATA wsadata;

        auto wsaStartUpResult = ::WSAStartup(wVersion, &wsadata);
        if (0 != wsaStartUpResult)
            return { false, std::format(L"was start up error: {}", wsaStartUpResult)};

        if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
        {
            ::WSACleanup();
            return { false, L"wsa start up: version 2.2 not exist." };
        }

        return { true, L"" };
    }

    void CleanWinSocketsEnvironment()
    {
        ::WSACleanup();
    }

    SocketCreateResult CreateTcpIpv4Socket()
    {
        int addressFamily = AF_INET;
        int socketType = SOCK_STREAM;
        int protocol = IPPROTO_TCP;

        SocketCreateResult result;

        result.socket = ::socket(addressFamily, socketType, protocol);

        if (result.socket == INVALID_SOCKET)
        {
            result.success = false;
            result.errorMessage = std::format(L"socket create failed, af = {}, type = {}. protocol = {}",
                                              addressFamily, socketType, protocol);
        }
        else
        {
            result.success = true;
            result.errorMessage = L"";
        }

        return result;
    }

    void CloseSocket(const SOCKET* pSocket)
    {
        if (*pSocket != INVALID_SOCKET)
            ::closesocket(*pSocket);
    }

    ActionResult SocketSend(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize)
    {
        auto sendResult = ::send(*pSocket, pDataBuffer, bufferSize, 0);
        if (sendResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket send error: {}", errorCode)};
        }

        return {true, L""};
    }

    SocketReceiveResult SocketReceive(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize)
    {
        // receiveResult < 0 -> error
        // receiveResult > 0 -> receive size
        auto receiveResult = ::recv(*pSocket, pDataBuffer, bufferSize, 0);
        if (receiveResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, 0, std::format(L"socket receive error: {}", errorCode)};
        }

        return {true, receiveResult, L""};
    }

    ActionResult SocketConnect(const SOCKET* pSocket, std::wstring ipStr, int port)
    {
        in_addr dst;
        ::InetPton(AF_INET, ipStr.c_str(), &dst);

        SOCKADDR_IN serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.S_un.S_addr = dst.S_un.S_addr;
        serverAddr.sin_port = ::htons(port);

        auto connectResult = ::connect(
                *pSocket,
                reinterpret_cast<SOCKADDR*>(&serverAddr),
                sizeof(SOCKADDR));

        if (connectResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket connect error: {}", errorCode)};
        }

        return {true, L""};
    }

    ActionResult SocketBind(const SOCKET* pSocket, std::wstring ipStr, int port)
    {
        in_addr dst;
        ::InetPton(AF_INET, ipStr.c_str(), &dst);

        SOCKADDR_IN serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.S_un.S_addr = dst.S_un.S_addr;
        serverAddr.sin_port = ::htons(port);

        auto bindResult = ::bind(
                *pSocket,
                reinterpret_cast<SOCKADDR*>(&serverAddr),
                sizeof(SOCKADDR));

        if (bindResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket bind error: {}", errorCode)};
        }

        return {true, L""};
    }

    ActionResult SocketListen(const SOCKET* pSocket)
    {
        auto bindResult = ::listen(*pSocket, SOMAXCONN);
        if (bindResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket listen error: {}", errorCode)};
        }

        return {true, L""};
    }

    SocketAcceptResult SocketAccept(const SOCKET* pSocket)
    {
        sockaddr_in addrIn {};
        int len = sizeof(sockaddr_in);
        auto acceptResult = ::accept(*pSocket, (sockaddr*)&addrIn, &len);
        if (acceptResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, addrIn, std::format(L"socket accept error: {}", errorCode)};
        }

        return {true, addrIn, L""};
    }

    SocketCreateEventResult SocketCreateEvent()
    {
        auto wsaEvent = ::WSACreateEvent();
        if (wsaEvent == WSA_INVALID_EVENT)
        {
            int errorCode = ::WSAGetLastError();
            return {false, WSA_INVALID_EVENT, std::format(L"socket accept error: {}", errorCode)};
        }

        return {true, wsaEvent, L""};
    }
}