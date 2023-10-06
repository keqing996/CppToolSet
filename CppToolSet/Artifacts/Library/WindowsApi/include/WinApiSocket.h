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

    struct SocketCreateResult: public ActionResult
    {
        SOCKET socket;

        SocketCreateResult(bool succ, SOCKET s, const std::wstring& errMsg);
    };

    struct SocketReceiveResult: public ActionResult
    {
        int receiveSize;

        SocketReceiveResult(bool succ, int size, const std::wstring& errMsg);
    };

    struct SocketAcceptResult: public ActionResult
    {
        sockaddr_in acceptAddr;

        SocketAcceptResult(bool succ, sockaddr_in addrIn, const std::wstring& errMsg);
    };

    struct SocketCreateEventResult: public ActionResult
    {
        WSAEVENT event;

        SocketCreateEventResult(bool succ, WSAEVENT e, const std::wstring& errMsg);
    };

    struct SocketEnumNetworkEventsResult: public ActionResult
    {
        WSANETWORKEVENTS  triggeredEvents;

        SocketEnumNetworkEventsResult(bool succ, WSANETWORKEVENTS events, const std::wstring& errMsg);

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

    SocketCreateEventResult SocketCreateEvent();

    void SocketCloseEvent(WSAEVENT* wsaEvent);

    ActionResult SocketEventSelect(const SOCKET* pSocket, WSAEVENT wsaEvent, long netEvent);

    DWORD SocketWaitForMultipleEvents(DWORD numberOfEvents, const WSAEVENT* pEventArray, DWORD timeOut = 0, bool waitAll = false, bool alertable = false);

    void SocketResetEvent(WSAEVENT wsaEvent);

    SocketEnumNetworkEventsResult SocketEnumNetworkEvents(const SOCKET* pSocket, WSAEVENT wsaEvent);

}