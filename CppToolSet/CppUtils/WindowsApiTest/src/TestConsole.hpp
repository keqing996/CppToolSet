#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include "WinApiConsole.h"

void TestConsole()
{
    auto handle = WindowsApi::Console::GetStdOutputHandle();
    WindowsApi::Console::SetWindowMaxEnable(handle, false);
    WindowsApi::Console::SetWindowMinEnable(handle, false);
    WindowsApi::Console::SetWindowResizeEnable(handle, false);
    WindowsApi::Console::SetTitle(L"2333");

    std::mutex mt;

    auto thread = std::thread([&mt](){
        for (int i = 0; i < 1000; i++)
        {
            std::lock_guard lock(mt);
            std::cout << "sssssssssssssssssssssssssssssssssssssssssssssssssssss" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds {1000});
        }
    });

    std::string str;
    while (true)
    {
        std::getline(std::cin, str);

        if (str == "exit")
            break;

        mt.lock();
        std::cout << str << std::endl;
        str = "";
        mt.unlock();
    }

    thread.join();

    std::cout << "test" << std::endl;
    std::cin.get();
}