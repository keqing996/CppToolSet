#pragma once

#include <iostream>
#include <Infra/Windows/WindowsDefine.hpp>
#include <Infra/Windows/Console.hpp>

void SetConsoleColor(Infra::Console::Color c)
{
    Infra::Console::SetStdOutColor(c, Infra::Console::Color::Black);
}

void LogError(const std::string& message)
{
    SetConsoleColor(Infra::Console::Color::Red);
    std::cout << message << std::endl;
    SetConsoleColor(Infra::Console::Color::None);
}

void LogError(const std::wstring& message)
{
    SetConsoleColor(Infra::Console::Color::Red);
    std::wcout << message << std::endl;
    SetConsoleColor(Infra::Console::Color::None);
}