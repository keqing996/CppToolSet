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

    std::wstring GetTitle();

    bool SetTitle(const std::wstring& title);

    CONSOLE_SCREEN_BUFFER_INFOEX GetScreenBufferInfo(HANDLE consoleHandle);

    Coord<short> GetBufferSize(HANDLE consoleHandle);

    Coord<short> GetCursorPosition(HANDLE consoleHandle);

    Rect<short> GetWindowRect(HANDLE consoleHandle);

    Coord<short> GetWindowSize(HANDLE consoleHandle);

    bool SetBufferSize(HANDLE consoleHandle, Coord<short> newSize);

    bool SetCursorPosition(HANDLE consoleHandle, Coord<short> newPos);

    bool SetWindowSize(HANDLE consoleHandle, Coord<short> size, bool absolute = false);

    void SetWindowResizeEnable(bool enable);

    void SetWindowMaxEnable(bool enable);

    void SetWindowMinEnable(bool enable);

    void SetColor(HANDLE consoleHandle, ConsoleColor foreground, ConsoleColor background, bool foregroundIntensity = false, bool backgroundIntensity = false);

    void ClearScreen();
}