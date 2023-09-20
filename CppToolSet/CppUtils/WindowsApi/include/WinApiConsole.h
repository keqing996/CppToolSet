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
}