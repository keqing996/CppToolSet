#pragma once

#include <string>
#include <format>
#include <fstream>
#include <mutex>

namespace Util
{
    class Logger
    {
    public:
        Logger() = delete;

    public:
        static void InitConsoleLogger();
        static void InitFileLogger(const std::string& loggerPath);
        static void Close();

    public:
        static void LogInfo(const std::string& message);
        static void LogInfo(const std::string_view& message);
        static void LogInfo(const char* message);

        template <class... Types>
        static void LogInfo(const std::format_string<Types...> Fmt, const Types&... Args)
        {
            LogInfo(std::format(Fmt, Args...));
        }

        static void LogWarn(const std::string& message);
        static void LogWarn(const std::string_view& message);
        static void LogWarn(const char* message);

        template <class... Types>
        static void LogWarn(const std::format_string<Types...> Fmt, const Types&... Args)
        {
            LogWarn(std::format(Fmt, Args...));
        }

        static void LogError(const std::string& message);
        static void LogError(const std::string_view& message);
        static void LogError(const char* message);

        template <class... Types>
        static void LogError(const std::format_string<Types...> Fmt, const Types&... Args)
        {
            LogError(std::format(Fmt, Args...));
        }

    private:
        static void WriteConsole(const std::string& message);
        static void WriteConsole(const std::string_view& message);
        static void WriteConsole(const char* message);

        static void WriteFile(const std::string& message);
        static void WriteFile(const std::string_view& message);
        static void WriteFile(const char* message);

    private:
        inline static bool _enableConsoleLogger = false;
        inline static bool _enableFileLogger = false;
        inline static std::mutex _mutex = {};
        inline static std::fstream* _pFileStream = nullptr;
    };
}
