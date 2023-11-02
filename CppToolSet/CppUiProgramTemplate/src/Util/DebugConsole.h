#pragma once

#include <string>
#include <format>

namespace Util
{
    class DebugConsole
    {
    public:
        DebugConsole() = delete;

    public:
        static void Init();

        static void LogInfo(const std::string& message);
        static void LogInfo(const std::string_view& message);
        static void LogInfo(const char* message);

        template <class... _Types>
        static void LogInfo(const std::format_string<_Types...> _Fmt, const _Types&... _Args)
        {
            LogInfo(std::format(_Fmt, _Args...));
        }

        static void LogWarn(const std::string& message);
        static void LogWarn(const std::string_view& message);
        static void LogWarn(const char* message);

        template <class... _Types>
        static void LogWarn(const std::format_string<_Types...> _Fmt, const _Types&... _Args)
        {
            LogWarn(std::format(_Fmt, _Args...));
        }

        static void LogError(const std::string& message);
        static void LogError(const std::string_view& message);
        static void LogError(const char* message);

        template <class... _Types>
        static void LogError(const std::format_string<_Types...> _Fmt, const _Types&... _Args)
        {
            LogError(std::format(_Fmt, _Args...));
        }
    };
}
