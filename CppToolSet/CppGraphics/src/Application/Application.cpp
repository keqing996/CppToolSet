#include "Application.h"

Application* Application::_instance = nullptr;

void Application::CreateInstance(int initHeight, int initWidth, const wchar_t* name)
{
    if (_instance != nullptr)
        _instance = new Application(initHeight, initWidth, name);
}

const Application* Application::GetInstance()
{
    return _instance;
}

Application::Application(int initHeight, int initWidth, const wchar_t* name)
    : _height(initHeight)
    , _width(initWidth)
    , _hInst(GetModuleHandle(nullptr))
{
}

void Application::InitWindow(const wchar_t* name)
{
    WNDCLASSEX wc;

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsg;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInst;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = WND_CLASS_NAME;
    wc.hIconSm = nullptr;

    ::RegisterClassExW(&wc);

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
}
