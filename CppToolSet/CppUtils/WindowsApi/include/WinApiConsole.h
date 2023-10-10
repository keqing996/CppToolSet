#pragma once

#include "WinApiBaseDefine.h"
#include <string>

namespace WindowsApi::Console
{
    enum class ConsoleColor
    {
        None,
        Green,
        Red,
        Blue,
        White,
        Black,
        Yellow,
        Purple,
        Cyan
    };

    HANDLE GetStdOutputHandle();

    HANDLE GetStdInputHandle();

    HANDLE GetStdErrorHandle();

    CONSOLE_SCREEN_BUFFER_INFOEX GetScreenBufferInfo(HANDLE consoleHandle);

    void SetWindowResizeEnable(bool enable);

    void SetWindowMaxEnable(bool enable);

    void SetWindowMinEnable(bool enable);

    void SetColor(HANDLE consoleHandle, ConsoleColor foreground, ConsoleColor background, bool foregroundIntensity = false, bool backgroundIntensity = false);

    void ClearScreen();
}