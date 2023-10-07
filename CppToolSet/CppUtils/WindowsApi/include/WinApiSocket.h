#pragma once

#include "WinApiBaseDefine.h"
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

        ActionResult(bool succ, const std::wstring& errMsg);
    };

    struct CreateSocketResult: public ActionResult
    {
        SOCKET socket;

        CreateSocketResult(bool succ, SOCKET s, const std::wstring& errMsg);
    };

    struct CreateSockAddrResult: public ActionResult
    {
        SOCKADDR_IN addr;

        CreateSockAddrResult(bool succ, SOCKADDR_IN addr, const std::wstring& errMsg);
    };

    struct ReceiveResult: public ActionResult
    {
        int receiveSize;

        ReceiveResult(bool succ, int size, const std::wstring& errMsg);
    };

    struct AcceptResult: public ActionResult
    {
        SOCKADDR_IN acceptAddr;

        AcceptResult(bool succ, sockaddr_in addrIn, const std::wstring& errMsg);
    };

    struct CreateEventResult: public ActionResult
    {
        WSAEVENT event;

        CreateEventResult(bool succ, WSAEVENT e, const std::wstring& errMsg);
    };

    struct EnumEventsResult: public ActionResult
    {
        WSANETWORKEVENTS  triggeredEvents;

        EnumEventsResult(bool succ, WSANETWORKEVENTS events, const std::wstring& errMsg);

        template<int FD, int FD_BIT>
        bool GetFdBitResult() const
        {
            return (triggeredEvents.lNetworkEvents & FD)
                   && (triggeredEvents.iErrorCode[FD_BIT] == 0);
        }

        bool IsAccept() const;

        bool IsWrite() const;

        bool IsRead() const;

        bool IsClose() const;
    };

    SOCKADDR_IN GenAddrFromIpv4(const std::wstring& ipStr, int port);

    std::pair<std::wstring, int> GetIpv4FromAddr(SOCKADDR_IN addr);

    ActionResult InitWinSocketsEnvironment();

    void CleanWinSocketsEnvironment();

    CreateSocketResult CreateTcpIpv4Socket();

    void CloseSocket(const SOCKET* pSocket);

    ActionResult Send(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize);

    ReceiveResult Receive(const SOCKET* pSocket, Byte* pDataBuffer, int bufferSize);

    ActionResult Connect(const SOCKET* pSocket, std::wstring ipStr, int port);

    ActionResult Bind(const SOCKET* pSocket, std::wstring ipStr, int port);

    ActionResult Listen(const SOCKET* pSocket);

    AcceptResult Accept(const SOCKET* pSocket);

    CreateEventResult SocketCreateEvent();

    void CloseEvent(WSAEVENT * wsaEvent);

    ActionResult EventSelect(const SOCKET* pSocket, WSAEVENT wsaEvent, long netEvent);

    DWORD WaitForMultipleEvents(DWORD numberOfEvents, const WSAEVENT* pEventArray, DWORD timeOut = 0, bool waitAll = false, bool alertable = false);

    void EventReset(WSAEVENT wsaEvent);

    EnumEventsResult EnumEvents(const SOCKET* pSocket, WSAEVENT wsaEvent);

}