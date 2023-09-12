#pragma once

#include <optional>
#include <memory>

#include "GlobalDef.h"

class Window
{
public:
    Window(int width, int height, const wchar_t* name, WNDPROC msgHandle)
            : _width(width)
            , _height(height)
            , _hInst(GetModuleHandle(nullptr))
    {

        WNDCLASSEX wc;

        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = msgHandle;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = _hInst;
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = WND_CLASS_NAME;
        wc.hIconSm = nullptr;

        RegisterClassExW(&wc);

        RECT rect;
        rect.left = 100;
        rect.right = width + rect.left;
        rect.top = 100;
        rect.bottom = height + rect.top;

        DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU;

        AdjustWindowRect(&rect, style, FALSE);

        // send self via WM_NCCREATE. build the relationship between instance pointer and win api.
        void* lParam = this;

        _hWnd = CreateWindowExW(
                0,
                WND_CLASS_NAME,
                name,
                style,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                rect.right - rect.left,
                rect.bottom - rect.top,
                nullptr,
                nullptr,
                _hInst,
                lParam
        );

        ShowWindow(_hWnd, SW_SHOWDEFAULT);
    }

    ~Window()
    {
        DestroyWindow(_hWnd);
        UnregisterClassW(WND_CLASS_NAME, _hInst);
    }

    Window(const Window&) = delete;

    Window& operator= (const Window&) = delete;

private:
    static constexpr LPCWSTR WND_CLASS_NAME = L"DX11 Engine";

private:
    int _width;
    int _height;
    HINSTANCE _hInst;
    HWND _hWnd;

public:
    int GetWidth() const
    {
        return _width;
    }

    int GetHeight() const
    {
        return _height;
    }

    HWND GetHWnd() const
    {
        return _hWnd;
    }

};

