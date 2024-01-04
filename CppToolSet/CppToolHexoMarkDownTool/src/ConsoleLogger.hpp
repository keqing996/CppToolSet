#pragma once

#include <iostream>
#include <Helper/WinApi/WindowsPlatform.h>
#include <Helper/WinApi/WinApiConsole.h>

HANDLE hConsoleHandle;

void InitConsoleLogger()
{
    hConsoleHandle = Helper::Win::Console::GetStdOutputHandle();
}

void SetConsoleColor(Helper::Win::Console::Color c)
{
    Helper::Win::Console::SetColor(hConsoleHandle, c, Helper::Win::Console::Color::Black);
}

void LogError(const std::string& message)
{
    SetConsoleColor(Helper::Win::Console::Color::Red);
    std::cout << message << std::endl;
    SetConsoleColor(Helper::Win::Console::Color::None);
}

void LogError(const std::wstring& message)
{
    SetConsoleColor(Helper::Win::Console::Color::Red);
    std::wcout << message << std::endl;
    SetConsoleColor(Helper::Win::Console::Color::None);
}