
#include "Logger.h"
#include "Util/WinApi/WinApiConsole.h"

#include <iostream>

namespace Util
{
    void Logger::SetFilterLevel(Logger::Level targetLevel)
    {
        _filterLevel = targetLevel;
    }

    Logger::Level Logger::GetCurrentFilterLevel()
    {
        return _filterLevel;
    }

    void Logger::InitConsoleLogger()
    {
#ifdef ENABLE_CONSOLE_LOG
        WinApi::Console::CreateConsole();
        WinApi::Console::SetConsoleOutputUtf8();
        _enableConsoleLogger = true;
#endif
    }

    void Logger::InitFileLogger(const std::string& loggerPath)
    {
        if (_pFileStream != nullptr)
            return;

        _pFileStream = new std::fstream(loggerPath, std::ios::out | std::ios::trunc);
        if (!_pFileStream->is_open())
        {
            delete _pFileStream;
            return;
        }

        _enableFileLogger = true;
    }

    void Logger::Close()
    {
        if (_pFileStream != nullptr)
        {
            if (_pFileStream->is_open())
                _pFileStream->close();

            delete _pFileStream;
        }

#ifdef ENABLE_CONSOLE_LOG
        WinApi::Console::DetachConsole();
#endif
    }

    void Logger::LogInfo(const std::string& message)
    {
        if (static_cast<int>(_filterLevel) > static_cast<int>(Logger::Level::Info))
            return;

        LogInfo(message.c_str());
    }

    void Logger::LogInfo(const char* message)
    {
        if (static_cast<int>(_filterLevel) > static_cast<int>(Logger::Level::Info))
            return;

        std::lock_guard<std::mutex> guard(_mutex);

        if (_enableConsoleLogger)
        {
            WinApi::Console::SetColor(WinApi::Console::ConsoleColor::White, WinApi::Console::ConsoleColor::None);
            std::cout << "[I] " << message << std::endl;
        }

        if (_enableFileLogger && _pFileStream != nullptr)
        {
            (*_pFileStream) << "[I] " << message << std::endl;
        }
    }

    void Logger::LogWarn(const std::string& message)
    {
        if (static_cast<int>(_filterLevel) > static_cast<int>(Logger::Level::Warning))
            return;

        LogWarn(message.c_str());
    }

    void Logger::LogWarn(const char* message)
    {
        if (static_cast<int>(_filterLevel) > static_cast<int>(Logger::Level::Warning))
            return;

        std::lock_guard<std::mutex> guard(_mutex);

        if (_enableConsoleLogger)
        {
            WinApi::Console::SetColor(WinApi::Console::ConsoleColor::Yellow, WinApi::Console::ConsoleColor::None);
            std::cout << "[W] " << message << std::endl;
            WinApi::Console::SetColor(WinApi::Console::ConsoleColor::White, WinApi::Console::ConsoleColor::None);
        }

        if (_enableFileLogger && _pFileStream != nullptr)
        {
            (*_pFileStream) << "[W] " << message << std::endl;
        }
    }

    void Logger::LogError(const std::string& message)
    {
        if (static_cast<int>(_filterLevel) > static_cast<int>(Logger::Level::Error))
            return;

        LogError(message.c_str());
    }

    void Logger::LogError(const char* message)
    {
        if (static_cast<int>(_filterLevel) > static_cast<int>(Logger::Level::Error))
            return;

        std::lock_guard<std::mutex> guard(_mutex);

        if (_enableConsoleLogger)
        {
            WinApi::Console::SetColor(WinApi::Console::ConsoleColor::Red, WinApi::Console::ConsoleColor::None);
            std::cout << "[E] " << message << std::endl;
            WinApi::Console::SetColor(WinApi::Console::ConsoleColor::White, WinApi::Console::ConsoleColor::None);
        }

        if (_enableFileLogger && _pFileStream != nullptr)
        {
            (*_pFileStream) << "[E] " << message << std::endl;
        }
    }
}