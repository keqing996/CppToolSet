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

    bool SetScreenBufferInfo(HANDLE consoleHandle, CONSOLE_SCREEN_BUFFER_INFOEX* pInfo);

    Coord<short> GetBufferSize(HANDLE consoleHandle);

    bool SetBufferSize(HANDLE consoleHandle, Coord<short> newSize);

    Coord<short> GetCursorPosition(HANDLE consoleHandle);

    bool SetCursorPosition(HANDLE consoleHandle, Coord<short> newPos);

    Rect<short> GetWindowRect(HANDLE consoleHandle);

    bool SetWindowRect(HANDLE consoleHandle, Rect<short> size);

    Coord<short> GetWindowSize(HANDLE consoleHandle);

    bool SetWindowSize(HANDLE consoleHandle, Coord<short> size);

    void SetWindowResizeEnable(HANDLE consoleHandle, bool enable);

    void SetWindowMaxEnable(HANDLE consoleHandle, bool enable);

    void SetWindowMinEnable(HANDLE consoleHandle, bool enable);

    void SetColor(HANDLE consoleHandle, ConsoleColor foreground, ConsoleColor background, bool foregroundIntensity = false, bool backgroundIntensity = false);

    void ClearScreen();
}