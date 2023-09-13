
#include "../include/WinApiSocket.h"

#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <format>

#pragma comment(lib, "Ws2_32.lib")

namespace WindowsApi
{

    Socket::Socket()
    {
    }

    Socket::~Socket()
    {
        if (_initSuccess)
            ::WSACleanup();
    }

    std::pair<bool, std::wstring> Socket::InitSocket(SocketMode mode)
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

        int addressFamily = AF_INET;
        int socketType = SOCK_STREAM;
        int protocol = IPPROTO_TCP;

        switch (mode)
        {
            case SocketMode::IPv4Tcp:
                addressFamily = AF_INET;
                socketType = SOCK_STREAM;
                protocol = IPPROTO_TCP;
                break;
            case SocketMode::IPv6Tcp:
                addressFamily = AF_INET6;
                socketType = SOCK_DGRAM;
                protocol = IPPROTO_TCP;
                break;
            case SocketMode::IPv4Udp:
                addressFamily = AF_INET;
                socketType = SOCK_STREAM;
                protocol = IPPROTO_UDP;
                break;
            case SocketMode::IPv6Udp:
                addressFamily = AF_INET6;
                socketType = SOCK_DGRAM;
                protocol = IPPROTO_UDP;
                break;
        }

        _socket = ::socket(addressFamily, socketType, protocol);

        if (_socket == INVALID_SOCKET)
        {
            ::WSACleanup();
            return { false, std::format(L"socket create failed, af = {}, type = {}. protocol = {}",
                                        addressFamily, socketType, protocol) };
        }

        _initSuccess = true;

        return {true, L""};
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

        _ip = ipStr;
        _port = port;

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

    std::wstring SocketClient::GetIp() const
    {
        return _ip;
    }

    int SocketClient::GetPort() const
    {
        return _port;
    }
}