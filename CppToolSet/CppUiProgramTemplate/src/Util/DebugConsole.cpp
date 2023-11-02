
#include "DebugConsole.h"

#ifdef BUILD_DEBUG
#   include <iostream>
#   include <WinApiConsole.h>
#endif


namespace Util
{
    void DebugConsole::Init()
    {
#ifdef BUILD_DEBUG
        WinApi::Console::CreateConsole();
        WinApi::Console::SetConsoleOutputUtf8();
#endif
    }

    void DebugConsole::LogInfo(const std::string& message)
    {
        std::cout << message << std::endl;
    }

    void DebugConsole::LogInfo(const std::string_view& message)
    {
        std::cout << message << std::endl;
    }

    void DebugConsole::LogInfo(const char* message)
    {
        std::cout << message << std::endl;
    }

    void DebugConsole::LogWarn(const std::string& message)
    {

    }

    static void LogWarn(const std::string_view& message)
    {

    }

    static void LogWarn(const char* message)
    {

    }

    static void LogError(const std::string& message)
    {

    }

    static void LogError(const std::string_view& message)
    {

    }

    static void LogError(const char* message)
    {

    }
}