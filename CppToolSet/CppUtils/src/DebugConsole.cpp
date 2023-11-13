
#include "../include/DebugConsole.h"

#include <iostream>

#ifdef BUILD_DEBUG
#   include "../include/WinApi/WinApiConsole.h"
#endif


namespace Util
{
    void Logger::InitConsoleLogger()
    {
#ifdef BUILD_DEBUG
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
    }

    void Logger::LogInfo(const std::string& message)
    {
        std::cout << message << std::endl;
    }

    void Logger::LogInfo(const std::string_view& message)
    {
        std::cout << message << std::endl;
    }

    void Logger::LogInfo(const char* message)
    {
        std::cout << message << std::endl;
    }

    void Logger::LogWarn(const std::string& message)
    {

    }

    void Logger::WriteConsole(const std::string& message)
    {
#ifdef BUILD_DEBUG
        if (!_enableConsoleLogger)
            return;

        std::lock_guard<std::mutex> guard(_mutex);
        std::cout << message << std::endl;
#endif
    }

    void Logger::WriteConsole(const std::string_view& message)
    {
#ifdef BUILD_DEBUG
        if (!_enableConsoleLogger)
            return;

        std::lock_guard<std::mutex> guard(_mutex);
        std::cout << message << std::endl;
#endif
    }

    void Logger::WriteConsole(const char* message)
    {
#ifdef BUILD_DEBUG
        if (!_enableConsoleLogger)
            return;

        std::lock_guard<std::mutex> guard(_mutex);
        std::cout << message << std::endl;
#endif
    }

    void Logger::WriteFile(const std::string& message)
    {
        if (!_enableFileLogger || _pFileStream == nullptr)
            return;

        std::lock_guard<std::mutex> guard(_mutex);
        (*_pFileStream) << message << std::endl;
    }

    void Logger::WriteFile(const std::string_view& message)
    {
        if (!_enableFileLogger || _pFileStream == nullptr)
            return;

        std::lock_guard<std::mutex> guard(_mutex);
        (*_pFileStream) << message << std::endl;
    }

    void Logger::WriteFile(const char* message)
    {
        if (!_enableFileLogger || _pFileStream == nullptr)
            return;

        std::lock_guard<std::mutex> guard(_mutex);
        (*_pFileStream) << message << std::endl;
    }
}