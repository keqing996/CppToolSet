
#include "../include/WinApiSocket.h"
#include <Windows.h>
#include <WinSock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <format>

#pragma comment(lib, "Ws2_32.lib")

namespace WinApi::Socket
{
    CreateSocketAddrResult CreateAddrFromIpv4(const std::wstring& ipStr, int port)
    {
        in_addr dst{};
        int result = ::InetPton(AF_INET, ipStr.c_str(), &dst);
        if (result != 1)
        {
            int errorCode = ::WSAGetLastError();
            return {false, {}, std::format(L"ip error: {}", errorCode)};
        }

        SOCKADDR_IN sockAddrIn;
        sockAddrIn.sin_family = AF_INET;
        sockAddrIn.sin_addr.S_un.S_addr = dst.S_un.S_addr;
        sockAddrIn.sin_port = ::htons(port);

        return {true, sockAddrIn, L""};
    }

    IpInfoResult GetIpv4FromAddr(SOCKADDR_IN addr)
    {
        wchar_t pIpStr[16] {0};
        auto pErrMsg = ::InetNtop(AF_INET, &addr.sin_addr.S_un.S_addr, pIpStr, sizeof(pIpStr));

        if (pErrMsg != nullptr)
        {
            return { false, L"", 0, {pErrMsg} };
        }

        auto port = ::ntohs(addr.sin_port);
        return { true, {pIpStr}, port, L""};
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

    CreateSocketResult CreateTcpIpv4Socket()
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

    ActionResult Send(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize)
    {
        auto sendResult = ::send(*pSocket, pDataBuffer, bufferSize, 0);
        if (sendResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket send error: {}", errorCode)};
        }

        return {true, L""};
    }

    ReceiveResult Receive(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize)
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

    ActionResult Connect(const SOCKET* pSocket, std::wstring ipStr, int port)
    {
        auto createAddrResult = CreateAddrFromIpv4(ipStr, port);
        if (!createAddrResult.success)
            return {false, createAddrResult.errorMessage};

        SOCKADDR_IN serverAddr = createAddrResult.result;

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

    ActionResult Bind(const SOCKET* pSocket, std::wstring ipStr, int port)
    {
        auto createAddrResult = CreateAddrFromIpv4(ipStr, port);
        if (!createAddrResult.success)
            return {false, createAddrResult.errorMessage};

        SOCKADDR_IN serverAddr = createAddrResult.result;

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

    ActionResult Listen(const SOCKET* pSocket)
    {
        auto bindResult = ::listen(*pSocket, SOMAXCONN);
        if (bindResult == SOCKET_ERROR)
        {
            int errorCode = ::WSAGetLastError();
            return {false, std::format(L"socket listen error: {}", errorCode)};
        }

        return {true, L""};
    }

    AcceptResult Accept(const SOCKET* pSocket)
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

    CreateEventResult SocketCreateEvent()
    {
        WSAEVENT wsaEvent = ::WSACreateEvent();
        if (wsaEvent == WSA_INVALID_EVENT)
        {
            int errorCode = ::WSAGetLastError();
            return {false, WSA_INVALID_EVENT, std::format(L"socket accept error: {}", errorCode)};
        }

        return {true, wsaEvent, L""};
    }

    bool EnumEventsIsAccept(const EnumEventsResult &result)
    {
        return GetEnumEventsFdBitResult<FD_ACCEPT, FD_ACCEPT_BIT>(result);
    }

    bool EnumEventsIsWrite(const EnumEventsResult &result)
    {
        return GetEnumEventsFdBitResult<FD_WRITE, FD_WRITE_BIT>(result);
    }

    bool EnumEventsIsRead(const EnumEventsResult &result)
    {
        return GetEnumEventsFdBitResult<FD_READ, FD_READ_BIT>(result);
    }

    bool EnumEventsIsClose(const EnumEventsResult &result)
    {
        return GetEnumEventsFdBitResult<FD_CLOSE, FD_CLOSE_BIT>(result);
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

    EnumEventsResult SocketEnumNetworkEvents(const SOCKET* pSocket, WSAEVENT wsaEvent)
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