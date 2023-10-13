
#include "Application.h"

bool Application::InitWindow(unsigned int windowWidth, unsigned int windowHeight, const wchar_t* name)
{
    _width = windowWidth;
    _height = windowHeight;

    _pWndClassEx = new WNDCLASSEX {};

    _pWndClassEx->cbSize = sizeof(*_pWndClassEx);
    _pWndClassEx->style = CS_OWNDC;
    _pWndClassEx->lpfnWndProc = HandleMsg;
    _pWndClassEx->cbClsExtra = 0;
    _pWndClassEx->cbWndExtra = 0;
    _pWndClassEx->hInstance = _hInst;
    _pWndClassEx->hIcon = nullptr;
    _pWndClassEx->hCursor = nullptr;
    _pWndClassEx->hbrBackground = nullptr;
    _pWndClassEx->lpszMenuName = nullptr;
    _pWndClassEx->lpszClassName = WND_CLASS_NAME;
    _pWndClassEx->hIconSm = nullptr;

    ::RegisterClassExW(_pWndClassEx);

    _pImGuiRenderer = new ImGuiRenderer();
    if (!_pImGuiRenderer->D3D11CreateDevice(_hWnd))
    {
        _pImGuiRenderer->D3D11CleanUpDevice();
        ::UnregisterClassW(_pWndClassEx->lpszClassName, _pWndClassEx->hInstance);
        return false;
    }

    RECT rect;
    rect.left = 100;
    rect.right = _width + rect.left;
    rect.top = 100;
    rect.bottom = _height + rect.top;

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU;

    ::AdjustWindowRect(&rect, style, FALSE);

    // send self via WM_NCCREATE. build the relationship between instance pointer and win api.
    void* lParam = this;

    _hWnd = ::CreateWindowExW(
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

    ::ShowWindow(_hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(_hWnd);

    // Dpi
    ::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // Im Gui Context
    _pImGuiRenderer->ImGuiCreateContext(_hWnd);
}

void Application::Destroy()
{
    if (_pImGuiRenderer != nullptr)
    {
        _pImGuiRenderer->ImGuiCleanUp();
        _pImGuiRenderer->D3D11CleanUpDevice();

        delete _pImGuiRenderer;

        _pImGuiRenderer = nullptr;
    }

    if (_pWndClassEx != nullptr)
    {
        ::DestroyWindow(_hWnd);
        ::UnregisterClassW(_pWndClassEx->lpszClassName, _pWndClassEx->hInstance);

        delete _pWndClassEx;

        _pWndClassEx = nullptr;
    }
}

void Application::RunLoop()
{
    while (true)
    {
        MSG msg;
        bool shouldStop = false;
        while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                shouldStop = true;
                break;
            }

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        if (shouldStop)
            break;

        _pImGuiRenderer->ImGuiRenderLoop();
    }
}

#pragma region [Getter]

int Application::GetWindowHeight() const
{
    return _width;
}

int Application::GetWindowWidth() const
{
    return _height;
}

#pragma endregion

#pragma region [Static Instance]

Application* Application::_instance = nullptr;

void Application::CreateInstance()
{
    if (_instance == nullptr)
        _instance = new Application();
}

Application* Application::GetInstance()
{
    return _instance;
}

#pragma endregion

#pragma region [Windows Message]

LRESULT Application::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return Application::GetInstance()->HandleMsgDispatch(hWnd, msg, wParam, lParam);
}

LRESULT Application::HandleMsgDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (_pImGuiRenderer != nullptr)
    {
        if (_pImGuiRenderer->ImGuiHandleWinMsg(hWnd, msg, wParam, lParam))
            return true;
    }

    switch (msg)
    {
        case WM_SIZE:
            return OnMsgWmSize(hWnd, msg, wParam, lParam);
        case WM_SYSCOMMAND:
            return OnMsgWmSysCommand(hWnd, msg, wParam, lParam);
        case WM_DESTROY:
        case WM_CLOSE:
            return OnMsgWmCloseAndDestroy(hWnd, msg, wParam, lParam);
        default:
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

LRESULT Application::OnMsgWmSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    _width = (UINT)LOWORD(lParam);
    _height = (UINT)HIWORD(lParam);
    if (wParam != SIZE_MINIMIZED && _pImGuiRenderer != nullptr)
        _pImGuiRenderer->D3D11OnResize(_width, _height);

    return 0;
}

LRESULT Application::OnMsgWmCloseAndDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ::PostQuitMessage(0);
    return 0;
}

LRESULT Application::OnMsgWmSysCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
        return 0;

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma endregion


