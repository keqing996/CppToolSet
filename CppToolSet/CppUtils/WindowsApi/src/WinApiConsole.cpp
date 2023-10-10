
#include "../include/WinApiConsole.h"


namespace WindowsApi::Console
{
    HWND GetWindowHandle()
    {
        return ::GetConsoleWindow();
    }

    HANDLE GetStdOutputHandle()
    {
        return ::GetStdHandle(STD_OUTPUT_HANDLE);
    }

    HANDLE GetStdInputHandle()
    {
        return ::GetStdHandle(STD_INPUT_HANDLE);
    }

    HANDLE GetStdErrorHandle()
    {
        return ::GetStdHandle(STD_ERROR_HANDLE);
    }

    std::wstring GetTitle()
    {
        wchar_t buf[0xFF];
        ::GetConsoleTitle(buf,0xFF);
        return { buf };
    }

    bool SetTitle(const std::wstring& title)
    {
        return ::SetConsoleTitle(title.c_str());
    }

    CONSOLE_SCREEN_BUFFER_INFOEX GetScreenBufferInfo(HANDLE consoleHandle)
    {
        CONSOLE_SCREEN_BUFFER_INFOEX info;
        info.cbSize = sizeof(info);

        ::GetConsoleScreenBufferInfoEx(consoleHandle, &info);

        // windows bug, https://stackoverflow.com/questions/35901572/setconsolescreenbufferinfoex-bug
        info.srWindow.Right++;
        info.srWindow.Bottom++;

        return info;
    }

    Coord<short> GetBufferSize(HANDLE consoleHandle)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        return { bufferInfo.dwSize.X, bufferInfo.dwSize.Y };
    }

    Coord<short> GetCursorPosition(HANDLE consoleHandle)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        return { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y };
    }

    Rect<short> GetWindowRect(HANDLE consoleHandle)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        return { bufferInfo.srWindow.Left, bufferInfo.srWindow.Top, bufferInfo.srWindow.Right, bufferInfo.srWindow.Bottom };
    }

    Coord<short> GetWindowSize(HANDLE consoleHandle)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        return { bufferInfo.srWindow.Right, bufferInfo.srWindow.Bottom };
    }

    bool SetBufferSize(HANDLE consoleHandle, Coord<short> newSize)
    {
        return ::SetConsoleScreenBufferSize(consoleHandle, { newSize.x, newSize.y });
    }

    bool SetCursorPosition(HANDLE consoleHandle, Coord<short> newPos)
    {
        return ::SetConsoleCursorPosition(consoleHandle, { newPos.x, newPos.y });
    }

    bool SetWindowSize(HANDLE consoleHandle, Coord<short> size, bool absolute)
    {
        size.x--;
        size.y--;
        SMALL_RECT winSize = {size.x, size.y};
        return ::SetConsoleWindowInfo(consoleHandle, absolute, &winSize);
    }

    void SetWindowResizeEnable(bool enable)
    {
        auto hWnd = GetWindowHandle();
        auto style= ::GetWindowLongPtr(hWnd, GWL_STYLE);
        style = style & (enable ? WS_SIZEBOX : ~WS_SIZEBOX);
        ::SetWindowLongPtr(hWnd, GWL_STYLE, style);
    }

    void SetWindowMaxEnable(bool enable)
    {
        auto hWnd = GetWindowHandle();
        auto style= ::GetWindowLongPtr(hWnd, GWL_STYLE);
        style = style & (enable ? WS_MAXIMIZEBOX : ~WS_MAXIMIZEBOX);
        ::SetWindowLongPtr(hWnd, GWL_STYLE, style);
    }

    void SetWindowMinEnable(bool enable)
    {
        auto hWnd = GetWindowHandle();
        auto style= ::GetWindowLongPtr(hWnd, GWL_STYLE);
        style = style & (enable ? WS_MINIMIZEBOX : ~WS_MINIMIZEBOX);
        ::SetWindowLongPtr(hWnd, GWL_STYLE, style);
    }

    void SetColor(HANDLE handle, ConsoleColor foreground, ConsoleColor background, bool foregroundIntensity, bool backgroundIntensity)
    {
        WORD result = 0;

        switch (foreground)
        {
            case ConsoleColor::None:
            case ConsoleColor::White:
                result |= FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN;
                break;
            case ConsoleColor::Black:
                result |= 0;
                break;
            case ConsoleColor::Green:
                result |= FOREGROUND_GREEN;
                break;
            case ConsoleColor::Blue:
                result |= FOREGROUND_BLUE;
                break;
            case ConsoleColor::Red:
                result |= FOREGROUND_RED;
                break;
            case ConsoleColor::Purple:
                result |= FOREGROUND_BLUE | FOREGROUND_RED;
                break;
            case ConsoleColor::Cyan:
                result |= FOREGROUND_BLUE | FOREGROUND_GREEN;
                break;
            case ConsoleColor::Yellow:
                result |= FOREGROUND_RED | FOREGROUND_GREEN;
                break;
            default:
                break;
        }

        if (foregroundIntensity)
            result |= FOREGROUND_INTENSITY;

        switch (background)
        {
            case ConsoleColor::None:
            case ConsoleColor::Black:
                result |= 0;
                break;
            case ConsoleColor::White:
                result |= BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN;
                break;
            case ConsoleColor::Green:
                result |= BACKGROUND_GREEN;
                break;
            case ConsoleColor::Blue:
                result |= BACKGROUND_BLUE;
                break;
            case ConsoleColor::Red:
                result |= BACKGROUND_RED;
                break;
            case ConsoleColor::Purple:
                result |= BACKGROUND_BLUE | BACKGROUND_RED;
                break;
            case ConsoleColor::Cyan:
                result |= BACKGROUND_BLUE | BACKGROUND_GREEN;
                break;
            case ConsoleColor::Yellow:
                result |= BACKGROUND_RED | BACKGROUND_GREEN;
                break;
            default:
                break;
        }

        if (backgroundIntensity)
            result |= BACKGROUND_INTENSITY;

        ::SetConsoleTextAttribute(handle, result);
    }

    // https://learn.microsoft.com/zh-cn/windows/console/clearing-the-screen
    void ClearScreen(HANDLE hConsole)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        SMALL_RECT scrollRect;
        COORD scrollTarget;
        CHAR_INFO fill;

        // Get the number of character cells in the current buffer.
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        {
            return;
        }

        // Scroll the rectangle of the entire buffer.
        scrollRect.Left = 0;
        scrollRect.Top = 0;
        scrollRect.Right = csbi.dwSize.X;
        scrollRect.Bottom = csbi.dwSize.Y;

        // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
        scrollTarget.X = 0;
        scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

        // Fill with empty spaces with the buffer's default text attribute.
        fill.Char.UnicodeChar = TEXT(' ');
        fill.Attributes = csbi.wAttributes;

        // Do the scroll
        ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

        // Move the cursor to the top left corner too.
        csbi.dwCursorPosition.X = 0;
        csbi.dwCursorPosition.Y = 0;

        SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
    }
}