
#include "../include/WinApiSocket.h"

#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <format>

#pragma comment(lib, "Ws2_32.lib")

namespace WindowsApi::Socket
{
    ActionResult::ActionResult(bool succ, const std::wstring &errMsg)
        : success(succ)
        , errorMessage(errMsg)
    {
    }

    SocketCreateResult::SocketCreateResult(bool succ, SOCKET s, const std::wstring &errMsg)
        : ActionResult(succ, errMsg)
        , socket(s)
    {
    }

    SocketReceiveResult::SocketReceiveResult(bool succ, int size, const std::wstring &errMsg)
        : ActionResult(succ, errMsg)
        , receiveSize(size)
    {
    }

    SocketAcceptResult::SocketAcceptResult(bool succ, sockaddr_in addrIn, const std::wstring &errMsg)
        : ActionResult(succ, errMsg)
        , acceptAddr(addrIn)
    {
    }

    SocketCreateEventResult::SocketCreateEventResult(bool succ, HANDLE e, const std::wstring &errMsg)
        : ActionResult(succ, errMsg)
        , event(e)
    {
    }

    SocketEnumNetworkEventsResult::SocketEnumNetworkEventsResult(bool succ, WSANETWORKEVENTS events, const std::wstring &errMsg)
        : ActionResult(succ, errMsg)
        , triggeredEvents(events)
    {
    }

    bool SocketEnumNetworkEventsResult::IsAccept() const
    {
        return GetFdBitResult<FD_ACCEPT, FD_ACCEPT_BIT>();
    }

    bool SocketEnumNetworkEventsResult::IsWrite() const
    {
        return GetFdBitResult<FD_WRITE, FD_WRITE_BIT>();
    }

    bool SocketEnumNetworkEventsResult::IsRead() const
    {
        return GetFdBitResult<FD_READ, FD_READ_BIT>();
    }

    bool SocketEnumNetworkEventsResult::IsClose() const
    {
        return GetFdBitResult<FD_CLOSE, FD_CLOSE_BIT>();
    }

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

        SOCKET socket = ::socket(addressFamily, socketType, protocol);

        if (socket == INVALID_SOCKET)
        {
            return {false, socket, std::format(L"socket create failed, af = {}, type = {}. protocol = {}",
                                               addressFamily, socketType, protocol)};
        }

        return {true, socket, L""};
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
        WSAEVENT wsaEvent = ::WSACreateEvent();
        if (wsaEvent == WSA_INVALID_EVENT)
        {
            int errorCode = ::WSAGetLastError();
            return {false, WSA_INVALID_EVENT, std::format(L"socket accept error: {}", errorCode)};
        }

        return {true, wsaEvent, L""};
    }

    void SocketCloseEvent(WSAEVENT* wsaEvent)
    {
        ::WSACloseEvent(*wsaEvent);
        *wsaEvent = WSA_INVALID_EVENT;
    }

    ActionResult SocketEventSelect(const SOCKET* pSocket, WSAEVENT wsaEvent, long e)
    {
        int eventSelectResult = ::WSAEventSelect(*pSocket, wsaEvent, e);
        if (eventSelectResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket event select error: {}", errorCode)};
        }

        return {true, L""};
    }

    DWORD SocketWaitForMultipleEvents(DWORD numberOfEvents, const WSAEVENT* pEventArray, DWORD timeOut, bool waitAll, bool alertable)
    {
        if (timeOut == 0)
            timeOut = WSA_INFINITE;

        return ::WSAWaitForMultipleEvents(numberOfEvents, pEventArray, waitAll, timeOut, alertable);
    }

    void SocketResetEvent(WSAEVENT wsaEvent)
    {
        ::WSAResetEvent(wsaEvent);
    }

    SocketEnumNetworkEventsResult SocketEnumNetworkEvents(const SOCKET* pSocket, WSAEVENT wsaEvent)
    {
        WSANETWORKEVENTS  triggeredEvents;
        int enumResult = ::WSAEnumNetworkEvents(*pSocket, wsaEvent, &triggeredEvents);
        if (enumResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, triggeredEvents, std::format(L"socket enum event error: {}", errorCode)};
        }

        return {true, triggeredEvents, L""};
    }
}