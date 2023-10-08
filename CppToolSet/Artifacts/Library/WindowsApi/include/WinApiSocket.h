#pragma once

#include "WinApiBaseDefine.h"
#include <string>
#include <thread>
#include <WinSock2.h>

namespace WindowsApi::Socket
{

#pragma region [Define]

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

    using ActionResult = ResultWithErrMsg<>;
    using CreateSocketResult = ResultWithErrMsg<SOCKET>;
    using CreateSocketAddrResult = ResultWithErrMsg<SOCKADDR_IN>;
    using ReceiveResult = ResultWithErrMsg<int>;
    using AcceptResult = ResultWithErrMsg<SOCKADDR_IN>;
    using CreateEventResult = ResultWithErrMsg<WSAEVENT>;
    using EnumEventsResult = ResultWithErrMsg<WSANETWORKEVENTS>;
    using IpInfoResult = ResultWithErrMsg<std::wstring, unsigned short>;

#pragma endregion

    CreateSocketAddrResult CreateAddrFromIpv4(const std::wstring& ipStr, int port);

    IpInfoResult GetIpv4FromAddr(SOCKADDR_IN addr);

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

#pragma region [Enum Events]

    EnumEventsResult EnumEvents(const SOCKET* pSocket, WSAEVENT wsaEvent);

    template<int FD, int FD_BIT>
    bool GetEnumEventsFdBitResult(const EnumEventsResult& result)
    {
        return (result.result.lNetworkEvents & FD)
               && (result.result.iErrorCode[FD_BIT] == 0);
    }

    bool EnumEventsIsAccept(const EnumEventsResult& result);

    bool EnumEventsIsWrite(const EnumEventsResult& result);

    bool EnumEventsIsRead(const EnumEventsResult& result);

    bool EnumEventsIsClose(const EnumEventsResult& result);

#pragma endregion

}