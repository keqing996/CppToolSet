
#include "../include/WinApiConsole.h"


namespace WindowsApi::Console
{
    HWND GetWindowHandle()
    {
        return ::GetConsoleWindow();
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

    bool ChangeSize(HANDLE consoleHandle, short width, short height)
    {
        SMALL_RECT wrt = { 0, 0, (short)(width - 1), (short)(height - 1) };
        return ::SetConsoleWindowInfo(consoleHandle, TRUE, &wrt);
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