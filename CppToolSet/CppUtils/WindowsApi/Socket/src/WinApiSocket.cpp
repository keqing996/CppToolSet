
#include "../include/WinApiSocket.h"

#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <format>

#pragma comment(lib, "Ws2_32.lib")

namespace WindowsApi
{

    Socket::Socket()
    {
        InitSocket();
    }

    Socket::~Socket()
    {
        if (_initSuccess)
            ::WSACleanup();
    }

    void Socket::InitSocket()
    {
        WORD wVersion = MAKEWORD(2, 2);
        WSADATA wsadata;

        if (0 != ::WSAStartup(wVersion, &wsadata))
            return;

        if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
        {
            ::WSACleanup();
            return;
        }

        _socket = ::socket(AF_INET, SOCK_STREAM, 0);

        _initSuccess = true;
    }

    std::pair<bool, std::wstring> Socket::Send(BYTE* dataBuffer, int bufferSize) const
    {
        auto checkResult = ActionCheck();
        if (!checkResult.first)
            return checkResult;

        auto sendResult = ::send(_socket, dataBuffer, bufferSize, 0);
        if (sendResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket error: {}", errorCode)};
        }

        return {true, L""};
    }

    std::pair<bool, std::wstring> Socket::Receive(Socket::BYTE* dataBuffer, int bufferSize, int* receiveSize)
    {
        auto checkResult = ActionCheck();
        if (!checkResult.first)
            return checkResult;

        int recvResult = ::recv(_socket, dataBuffer, bufferSize, 0);
        if (recvResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket error: {}", errorCode)};
        }

        *receiveSize = recvResult;
        return {true, L""};
    }

    std::pair<bool, std::wstring> Socket::ActionCheck() const
    {
        if (!_initSuccess)
            return {false, L"socket is not init."};

        return {true, L""};
    }

    bool Socket::HasInit() const
    {
        return _initSuccess;
    }

    SocketClient::SocketClient() : Socket()
    {
    }

    SocketClient::~SocketClient()
    {
        if (_connectSuccess)
            ::closesocket(_socket);

        Socket::~Socket();
    }

    std::pair<bool, std::wstring> SocketClient::Connect(std::wstring ipStr, int port)
    {
        if (!_initSuccess)
            return {false, L"socket not init."};

        in_addr dst;
        ::InetPton(AF_INET, ipStr.c_str(), &dst);

        SOCKADDR_IN serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.S_un.S_addr = dst.S_un.S_addr;
        serverAddr.sin_port = ::htons(port);

        auto connectResult = ::connect(
                _socket,
                reinterpret_cast<SOCKADDR*>(&serverAddr),
                sizeof(SOCKADDR));

        if (connectResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket error: {}", errorCode)};
        }

        _connectSuccess = true;
        return {true, L""};
    }

    std::pair<bool, std::wstring> SocketClient::ActionCheck() const
    {
        auto [baseSuccess, baseErrorStr] = Socket::ActionCheck();
        if (!baseSuccess)
            return {baseSuccess, baseErrorStr};

        if (!_connectSuccess)
            return {false, L"socket is not connected."};

        return {true, L""};
    }
}