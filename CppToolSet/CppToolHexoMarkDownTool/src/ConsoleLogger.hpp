#pragma once

#include <iostream>

#include "WinApi/WindowsPlatform.h"
#include "WinApi/WinApiConsole.h"

HANDLE hConsoleHandle;

void InitConsoleLogger()
{
    hConsoleHandle = WinApi::Console::GetStdOutputHandle();
}

void SetConsoleColor(WinApi::Console::ConsoleColor c)
{
    WinApi::Console::SetColor(hConsoleHandle, c, WinApi::Console::ConsoleColor::Black);
}

void LogError(const std::string& message)
{
    SetConsoleColor(WinApi::Console::ConsoleColor::Red);
    std::cout << message << std::endl;
    SetConsoleColor(WinApi::Console::ConsoleColor::None);
}

void LogError(const std::wstring& message)
{
    SetConsoleColor(WinApi::Console::ConsoleColor::Red);
    std::wcout << message << std::endl;
    SetConsoleColor(WinApi::Console::ConsoleColor::None);
}