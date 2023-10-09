
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

    bool SetConsoleBufferInfo(HANDLE consoleHandle, CONSOLE_SCREEN_BUFFER_INFOEX* pInfo)
    {
        return ::SetConsoleScreenBufferInfoEx(consoleHandle, pInfo);
    }

    Coord<short> GetBufferSize(HANDLE consoleHandle)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        return { bufferInfo.dwSize.X, bufferInfo.dwSize.Y };
    }

    bool SetBufferSize(HANDLE consoleHandle, Coord<short> newSize)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        bufferInfo.dwSize = { newSize.x, newSize.y };
        return SetScreenBufferInfo(consoleHandle, &bufferInfo);
    }

    Coord<short> GetCursorPosition(HANDLE consoleHandle)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        return { bufferInfo.dwCursorPosition.X, bufferInfo.dwCursorPosition.Y };
    }

    bool SetCursorPosition(HANDLE consoleHandle, Coord<short> newPos)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        bufferInfo.dwCursorPosition = { newPos.x, newPos.y };
        return SetScreenBufferInfo(consoleHandle, &bufferInfo);
    }

    Rect<short> GetWindowRect(HANDLE consoleHandle)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        return { bufferInfo.srWindow.Left, bufferInfo.srWindow.Top, bufferInfo.srWindow.Right, bufferInfo.srWindow.Bottom };
    }

    bool SetWindowRect(HANDLE consoleHandle, Rect<short> rect)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        rect.right--;
        rect.bottom--;
        bufferInfo.srWindow = {rect.left, rect.top, rect.right, rect.bottom };
        return SetScreenBufferInfo(consoleHandle, &bufferInfo);
    }

    Coord<short> GetWindowSize(HANDLE consoleHandle)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        return { bufferInfo.srWindow.Right, bufferInfo.srWindow.Bottom };
    }

    bool SetWindowSize(HANDLE consoleHandle, Coord<short> size)
    {
        auto bufferInfo = GetScreenBufferInfo(consoleHandle);
        size.x--;
        size.y--;
        bufferInfo.srWindow.Right = size.x;
        bufferInfo.srWindow.Bottom = size.y;
        return SetScreenBufferInfo(consoleHandle, &bufferInfo);
    }

    void SetWindowResizeEnable(HANDLE consoleHandle, bool enable)
    {
        auto hWnd = GetWindowHandle();
        auto style= ::GetWindowLongPtr(hWnd, GWL_STYLE);
        style = style & (enable ? WS_SIZEBOX : ~WS_SIZEBOX);
        ::SetWindowLongPtr(hWnd, GWL_STYLE, style);
    }

    void SetWindowMaxEnable(HANDLE consoleHandle, bool enable)
    {
        auto hWnd = GetWindowHandle();
        auto style= ::GetWindowLongPtr(hWnd, GWL_STYLE);
        style = style & (enable ? WS_MAXIMIZEBOX : ~WS_MAXIMIZEBOX);
        ::SetWindowLongPtr(hWnd, GWL_STYLE, style);
    }

    void SetWindowMinEnable(HANDLE consoleHandle, bool enable)
    {
        auto hWnd = GetWindowHandle();
        auto style= ::GetWindowLongPtr(hWnd, GWL_STYLE);
        style = style & (enable ? WS_MINIMIZEBOX : ~WS_MINIMIZEBOX);
        ::SetWindowLongPtr(hWnd, GWL_STYLE, style);
    }
}