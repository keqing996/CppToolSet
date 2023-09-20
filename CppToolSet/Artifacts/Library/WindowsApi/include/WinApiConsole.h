#pragma once

#include "WinApiBaseDefine.h"
#include <string>

namespace WindowsApi::Console
{
    std::wstring ConsoleGetTitle();

    bool ConsoleSetTitle(const std::wstring& title);

    HANDLE GetStdOutputConsoleHandle();

    HANDLE GetStdInputConsoleHandle();

    HANDLE GetStdErrorConsoleHandle();

    bool ChangeSize(HANDLE consoleHandle, short width, short height);

    void SetWindowResizeEnable(HANDLE consoleHandle, bool enable);

    void SetWindowMaxEnable(HANDLE consoleHandle, bool enable);

    void SetWindowMinEnable(HANDLE consoleHandle, bool enable);
}