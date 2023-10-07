#pragma once

#include "WinApiBaseDefine.h"
#include <string>
#include <thread>
#include <WinSock2.h>

namespace WindowsApi::Socket
{
    using Byte = char;

    template<typename... Args>
    struct ResultWithErrMsg;

    template<>
    struct ResultWithErrMsg<>
    {
        bool success;
        std::wstring errorMessage;

        ResultWithErrMsg(bool succ, const std::wstring& errMsg)
                : success(succ)
                , errorMessage(errMsg)
        {
        }
    };

    template<typename OnePara>
    struct ResultWithErrMsg<OnePara>
    {
        OnePara result;
        bool success;
        std::wstring errorMessage;

        ResultWithErrMsg(bool succ, OnePara para, const std::wstring& errMsg)
                : success(succ)
                , result(para)
                , errorMessage(errMsg)
        {
        }
    };

    template<typename Para1, typename Para2>
    struct ResultWithErrMsg<Para1, Para2>
    {
        Para1 result1;
        Para2 result2;
        bool success;
        std::wstring errorMessage;

        ResultWithErrMsg(bool succ, Para1 para1, Para2 para2, const std::wstring& errMsg)
                : success(succ)
                , result1(para1)
                , result2(para2)
                , errorMessage(errMsg)
        {
        }
    };

    struct ActionResult: public ResultWithErrMsg<>
    {
    };

    struct CreateSocketResult: public ResultWithErrMsg<SOCKET>
    {
    };

    struct CreateSocketAddrResult: public ResultWithErrMsg<SOCKADDR_IN>
    {
    };

    struct ReceiveResult: public ResultWithErrMsg<int>
    {
        int GetReceiveSize() const
        {
            return result;
        }
    };

    struct AcceptResult: public ResultWithErrMsg<SOCKADDR_IN>
    {
    };

    struct CreateEventResult: public ResultWithErrMsg<WSAEVENT>
    {
    };

    struct EnumEventsResult: public ResultWithErrMsg<WSANETWORKEVENTS>
    {
        template<int FD, int FD_BIT>
        bool GetFdBitResult() const
        {
            return (result.lNetworkEvents & FD)
                   && (result.iErrorCode[FD_BIT] == 0);
        }

        bool IsAccept() const
        {
            return GetFdBitResult<FD_ACCEPT, FD_ACCEPT_BIT>();
        }

        bool IsWrite() const
        {
            return GetFdBitResult<FD_WRITE, FD_WRITE_BIT>();
        }

        bool IsRead() const
        {
            return GetFdBitResult<FD_READ, FD_READ_BIT>();
        }

        bool IsClose() const
        {
            return GetFdBitResult<FD_CLOSE, FD_CLOSE_BIT>();
        }
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