#pragma once

#include "WinApiBaseDefine.h"
#include <string>

namespace WindowsApi::Console
{
    std::wstring GetTitle();

    bool SetTitle(const std::wstring& title);

    HANDLE GetStdOutputHandle();

    HANDLE GetStdInputHandle();

    HANDLE GetStdErrorHandle();

    bool ChangeSize(HANDLE consoleHandle, short width, short height);

    void SetWindowResizeEnable(HANDLE consoleHandle, bool enable);

    void SetWindowMaxEnable(HANDLE consoleHandle, bool enable);

    void SetWindowMinEnable(HANDLE consoleHandle, bool enable);
}