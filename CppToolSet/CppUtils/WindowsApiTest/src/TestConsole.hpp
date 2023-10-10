#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include "WinApiConsole.h"

using namespace WindowsApi::Console;

void TestConsole()
{
    auto handle = GetStdOutputHandle();
    SetWindowMaxEnable(handle, false);
    SetWindowMinEnable(handle, false);
    SetWindowResizeEnable(handle, false);
    SetTitle(L"2333");

    SetWindowSize(handle, {800, 400});

    std::cout << "Hello None" << std::endl;

    // foreground color

    SetColor(handle, ConsoleColor::White, ConsoleColor::Black);
    std::cout << "Hello Foreground White" << std::endl;

    SetColor(handle, ConsoleColor::Black, ConsoleColor::Black);
    std::cout << "Hello Foreground Black" << std::endl;

    SetColor(handle, ConsoleColor::Green, ConsoleColor::Black);
    std::cout << "Hello Foreground Green" << std::endl;

    SetColor(handle, ConsoleColor::Blue, ConsoleColor::Black);
    std::cout << "Hello Foreground Blue" << std::endl;

    SetColor(handle, ConsoleColor::Red, ConsoleColor::Black);
    std::cout << "Hello Foreground Red" << std::endl;

    SetColor(handle, ConsoleColor::Cyan, ConsoleColor::Black);
    std::cout << "Hello Foreground Cyan" << std::endl;

    SetColor(handle, ConsoleColor::Yellow, ConsoleColor::Black);
    std::cout << "Hello Foreground Yellow" << std::endl;

    SetColor(handle, ConsoleColor::Purple, ConsoleColor::Black);
    std::cout << "Hello Foreground Purple" << std::endl;

    // background color

    SetColor(handle, ConsoleColor::White, ConsoleColor::White);
    std::cout << "Hello Background White" << std::endl;

    SetColor(handle, ConsoleColor::White, ConsoleColor::Black);
    std::cout << "Hello Background Black" << std::endl;

    SetColor(handle, ConsoleColor::White, ConsoleColor::Green);
    std::cout << "Hello Background Green" << std::endl;

    SetColor(handle, ConsoleColor::White, ConsoleColor::Blue);
    std::cout << "Hello Background Blue" << std::endl;

    SetColor(handle, ConsoleColor::White, ConsoleColor::Red);
    std::cout << "Hello Background Red" << std::endl;

    SetColor(handle, ConsoleColor::White, ConsoleColor::Cyan);
    std::cout << "Hello Background Cyan" << std::endl;

    SetColor(handle, ConsoleColor::White, ConsoleColor::Yellow);
    std::cout << "Hello Background Yellow" << std::endl;

    SetColor(handle, ConsoleColor::White, ConsoleColor::Purple);
    std::cout << "Hello Background Purple" << std::endl;

    std::cin.get();
}