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

    ActionResult InitWinSocketsEnvironment();

    void CleanWinSocketsEnvironment();

    SocketCreateResult CreateTcpIpv4Socket();

    void CloseSocket(const SOCKET* pSocket);

    ActionResult SocketSend(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize);

    SocketReceiveResult SocketReceive(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize);

    ActionResult SocketConnect(const SOCKET* pSocket, std::wstring ipStr, int port);

    ActionResult SocketBind(const SOCKET* pSocket, std::wstring ipStr, int port);

    ActionResult SocketListen(const SOCKET* pSocket);

/*

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

    class SocketServer : public Socket
    {
    public:
        SocketServer();
        ~SocketServer();

    public:
        std::pair<bool, std::wstring> Bind(std::wstring ipStr, int port);
        std::pair<bool, std::wstring> Listen();
        std::wstring GetIp() const;
        int GetPort() const;

        void Start();

    protected:
        std::pair<bool, std::wstring> ActionCheck() const override;

    protected:
        std::wstring _ip;
        int _port;
        bool _bindSuccess = false;

        SOCKET _clientSocketArray[64];
        WSAEVENT _clientSocketEventArray[64];
        unsigned int _clientSocketNum = 0;

        std::thread _mainThread;
        std::thread _processThread;
    };

    */
}