#pragma once

#include "WinApiBaseDefine.h"
#include <string>
#include <thread>
#include <optional>

namespace WinApi::Socket
{
    using Byte = char;
    using SocketHandle = void*;
    using WsaEventHandle = void*;

    enum class EventType
    {
        Read,
        Write,
        Accept,
        Connect,
        Close
    };

    struct SocketAddr
    {
        std::string ip;
        unsigned int port;
    };

    struct EnumEventResult
    {
        bool read;
        bool write;
        bool accept;
        bool connect;
        bool close;
    };

    const std::string& LastErrorMessage();

    bool StartUp();

    void CleanUp();

    std::optional<SocketHandle> CreateIpv4TcpSocket();

    void CloseSocket(SocketHandle socket);

    bool Send(SocketHandle socket, Byte* pDataBuffer, int bufferSize);

    std::optional<int> Receive(SocketHandle socket, Byte* pDataBuffer, int bufferSize);

    bool Connect(SocketHandle socket, const std::string& ipStr, int port);

    bool Bind(SocketHandle socket, const std::string& ipStr, int port);

    bool Listen(SocketHandle socket);

    std::optional<SocketAddr> Accept(SocketHandle socket);

    std::optional<WsaEventHandle> CreateWsaEvent();

    void CloseWsaEvent(WsaEventHandle wsaEvent);

    bool WsaEventSelect(SocketHandle socket, WsaEventHandle wsaEvent, const EventType* events, uint32_t eventNum);

    void WsaEventReset(WsaEventHandle wsaEvent);

    uint32_t WaitForMultipleWsaEvents(const WsaEventHandle* pEventArray, uint32_t numberOfEvents, uint32_t timeOut = 0, bool waitAll = false, bool alertable = false);

    uint32_t GetWsaEnumEventBaseIndex();

    std::optional<EnumEventResult> SocketEnumNetworkEvents(SocketHandle socket, WsaEventHandle wsaEvent);

}