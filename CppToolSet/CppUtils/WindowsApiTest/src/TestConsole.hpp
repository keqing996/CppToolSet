#pragma once

#include <iostream>
#include "WinApiConsole.h"

void TestConsole()
{
    auto handle = WindowsApi::Console::GetStdOutputHandle();
    WindowsApi::Console::SetWindowMaxEnable(handle, false);
    WindowsApi::Console::SetWindowMinEnable(handle, false);
    WindowsApi::Console::SetWindowResizeEnable(handle, false);
    WindowsApi::Console::SetTitle(L"2333");
    std::cout << "test" << std::endl;
    std::cin.get();
}