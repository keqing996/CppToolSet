#pragma once

#include <string>

namespace WinApi::Console
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

    void CreateConsole();

    void AttachConsole();

    void DetachConsole();

    void SetConsoleOutputUtf8();

    void* GetStdOutputHandle();

    void* GetStdInputHandle();

    void* GetStdErrorHandle();

    void SetWindowResizeEnable(bool enable);

    void SetWindowMaxEnable(bool enable);

    void SetWindowMinEnable(bool enable);

    void SetColor(ConsoleColor foreground, ConsoleColor background, bool foregroundIntensity = false, bool backgroundIntensity = false);

    void SetColor(void* consoleHandle, ConsoleColor foreground, ConsoleColor background, bool foregroundIntensity = false, bool backgroundIntensity = false);

    void ClearScreen();
}