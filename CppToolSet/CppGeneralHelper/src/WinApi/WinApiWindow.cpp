#include "../../include/Helper/WinApi/WindowsPlatform.h"
#include "../../include/Helper/WinApi/WinApiWindow.h"

namespace Helper::Win::Window
{
    static LRESULT CALLBACK DefaultWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg) {
            case WM_CLOSE:
                ::PostQuitMessage(0);
                return 0;
            case WM_DESTROY:
                return 0;
            default:
                return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    void Register(const std::wstring& windowRegisterName, RegisterInfo info)
    {
        const WNDCLASSEXW wc = {
            sizeof(wc),
            CS_CLASSDC,
            static_cast<WNDPROC>(info.pWinMsgProc),
            0L,
            0L,
            ::GetModuleHandle(nullptr),
            static_cast<HICON>(info.hIcon),
            static_cast<HICON>(info.hCursor),
            nullptr,
            nullptr,
            windowRegisterName.c_str(),
            static_cast<HICON>(info.hIconSmall),
        };

        ::RegisterClassExW(&wc);
    }

    void* Show(const std::wstring& windowRegisterName, const std::wstring& windowTitleName, int width, int height, CreateStyle style)
    {
        DWORD windowStyle = WS_OVERLAPPED;
        if (style.hasCaption)
        {
            windowStyle |= WS_CAPTION;

            if (style.hasSysmenu)
            {
                windowStyle |= WS_SYSMENU;

                if (style.hasMaxBtn)
                    windowStyle |= WS_MAXIMIZEBOX;

                if (style.hasMinBtn)
                    windowStyle |= WS_MINIMIZEBOX;
            }
        }

        HWND hWnd = ::CreateWindowW(
                windowRegisterName.c_str(),
                windowTitleName.c_str(),
                windowStyle,
                100,
                100,
                width,
                height,
                nullptr,
                nullptr,
                ::GetModuleHandle(nullptr),
                nullptr);

        ::SetWindowTextW(hWnd, windowTitleName.c_str());
        ::ShowWindow(hWnd, SW_SHOWDEFAULT);
        ::UpdateWindow(hWnd);

        return (void*)hWnd;
    }

    void Destroy(void* hWnd)
    {
        ::DestroyWindow(static_cast<HWND>(hWnd));
    }

    void UnRegister(const std::wstring& windowRegisterName)
    {
        ::UnregisterClassW(windowRegisterName.c_str(), GetModuleHandle(nullptr));
    }

    bool MessageLoop(bool blockWhenNoWindowsMessage)
    {
        bool shouldQuit = false;
        MSG msg;

        if (blockWhenNoWindowsMessage)
        {
            // When GetMessage() retrieves a WM_QUIT message from the queue, it will return a value of 0
            if (::GetMessage(&msg, nullptr, 0U, 0U) == 0)
            {
                shouldQuit = true;
            }
            else
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        else
        {
            while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);

                shouldQuit = msg.message == WM_QUIT;
                if (shouldQuit)
                    break;
            }
        }

        return shouldQuit;
    }

    void* GetDefaultWinMsgProc()
    {
        return &DefaultWindowProc;
    }
}