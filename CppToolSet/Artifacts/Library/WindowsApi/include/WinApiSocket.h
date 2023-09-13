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
#include <WinSock2.h>

namespace WindowsApi
{
    enum class SocketMode: int
    {
        IPv4Tcp,
        IPv6Tcp,
        IPv4Udp,
        IPv6Udp,
    };

    class Socket
    {
        using BYTE = char;
    public:
        Socket();
        virtual ~Socket();

    public:
        bool HasInit() const;
        std::pair<bool, std::wstring> InitSocket(SocketMode mode);
        std::pair<bool, std::wstring> Send(BYTE* dataBuffer, int bufferSize) const;
        std::pair<bool, std::wstring> Receive(BYTE* dataBuffer, int bufferSize, int* receiveSize);

    protected:
        virtual std::pair<bool, std::wstring> ActionCheck() const;

    protected:
        bool _initSuccess = false;

        SOCKET _socket;
    };

    class SocketClient : public Socket
    {
    public:
        SocketClient();
        ~SocketClient() override;

    public:
        std::pair<bool, std::wstring> Connect(std::wstring ipStr, int port);
        std::wstring GetIp() const;
        int GetPort() const;

    protected:
        std::pair<bool, std::wstring> ActionCheck() const override;

    protected:
        std::wstring _ip;
        int _port;
        bool _connectSuccess = false;
    };
}