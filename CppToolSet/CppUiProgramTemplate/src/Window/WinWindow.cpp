#include "WinWindow.h"
#include "Define/WindowsPlatform.h"

namespace UI
{
    Win32Window::Win32Window(const char* windowRegisterName, const char* windowTitle, int width, int height)
        : _windowRegisterName(windowRegisterName)
        , _width(width)
        , _height(height)
    {
        WNDCLASSEX wc = {
                sizeof(wc),
                CS_CLASSDC,
                WndProcDispatch,
                0L,
                0L,
                GetModuleHandle(nullptr),
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                windowRegisterName,
                nullptr
        };

        ::RegisterClassEx(&wc);

        _hWnd = ::CreateWindow(
                wc.lpszClassName,
                windowTitle,
                WS_OVERLAPPEDWINDOW,
                100,
                100,
                width,
                height,
                nullptr,
                nullptr,
                wc.hInstance,
                this);
    }

    Win32Window::~Win32Window()
    {
        ::DestroyWindow(_hWnd);
        ::UnregisterClass(_windowRegisterName, GetModuleHandle(nullptr));
    }

    void Win32Window::Show()
    {
        ::ShowWindow(_hWnd, SW_SHOWDEFAULT);
        ::UpdateWindow(_hWnd);
    }

    void Update(bool* isQuit)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            *isQuit = msg.message == WM_QUIT;
            if (*isQuit)
                break;
        }
    }

    HWND Win32Window::GetWindowHandle() const
    {
        return _hWnd;
    }

    LRESULT Win32Window::WndProcDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Win32Window* pThis = nullptr;
        if (msg == WM_CREATE)
        {
            auto longPointerCreateStructure = reinterpret_cast<LPCREATESTRUCTA>(lParam);
            pThis = static_cast<Win32Window*>(longPointerCreateStructure->lpCreateParams);
            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        }
        else
        {
            pThis = reinterpret_cast<Win32Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }

        if (pThis != nullptr)
            pThis->OnWinMsg(hWnd, msg, wParam, lParam);
        else
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }

    void Win32Window::AddWinMsgProc(IWinMsgReceiver* pWinMsgReceiver)
    {
        auto itr = std::find(_winMsgReceiverVec.begin(), _winMsgReceiverVec.end(), pWinMsgReceiver);
        if (itr != _winMsgReceiverVec.end())
            _winMsgReceiverVec.push_back(pWinMsgReceiver);
    }

    void Win32Window::RemoveWinMsgProc(IWinMsgReceiver* pWinMsgReceiver)
    {
        auto itr = std::find(_winMsgReceiverVec.begin(), _winMsgReceiverVec.end(), pWinMsgReceiver);
        if (itr != _winMsgReceiverVec.end())
            _winMsgReceiverVec.erase(itr);
    }

    LRESULT Win32Window::OnWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (!_winMsgReceiverVec.empty())
        {
            for (auto pWinMsgReceiver : _winMsgReceiverVec)
            {
                if (pWinMsgReceiver != nullptr)
                    pWinMsgReceiver->OnWinMsg(hWnd, msg, wParam, lParam);
            }
        }

        switch (msg)
        {
            case WM_SIZE:
                return OnWinMsgSize(hWnd, msg, wParam, lParam);
            case WM_SYSCOMMAND:
                return OnWinMsgSysCommand(hWnd, msg, wParam, lParam);
            case WM_DESTROY:
                return OnWinMsgDestroy(hWnd, msg, wParam, lParam);
            default:
                return ::DefWindowProcW(hWnd, msg, wParam, lParam);
        }
    }

    LRESULT Win32Window::OnWinMsgSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (wParam == SIZE_MINIMIZED)
            return FALSE;

        _width = (int)LOWORD(lParam);
        _height = (int)HIWORD(lParam);

        return ::DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    LRESULT Win32Window::OnWinMsgSysCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return FALSE;

        return ::DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    LRESULT Win32Window::OnWinMsgDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        ::PostQuitMessage(0);
        return FALSE;
    }
}
