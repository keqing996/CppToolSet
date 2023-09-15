#include "Application.h"
#include "../../../../CppGraphics/src/Application/Application.h"

#include <string>

Application::Application(int windowWidth, int windowHeight, const wchar_t* windowName)
        : _window(windowWidth, windowHeight, windowName, HandleMsgSetup)
{
    _width = windowWidth;
    _height = windowHeight;
    _targetFrame = 120;
}

int Application::Begin()
{
    _timer.SetNow();

    while (true)
    {
        if (const auto msgResult = ProcessMessage())
        {
            return *msgResult;
        }

        float dt = static_cast<float>(_timer.GetIntervalMsAndSetNow());

        float consume = static_cast<float>(_timer.GetInterval());
        float min = 1000 / static_cast<float>(_targetFrame);
        while (consume < min)
        {
            consume = static_cast<float>(_timer.GetInterval());
        }

        auto title = std::to_wstring(dt);
        SetWindowText(_window.GetHWnd(), title.c_str());
    }
}

std::optional<int> Application::ProcessMessage()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return static_cast<int>(msg.wParam);

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

LRESULT Application::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Application* const pApp = static_cast<Application*>(pCreate->lpCreateParams);

        // put window instance pointer into Windows api side,
        // so we can always get window instance through hWnd
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));

        // set process function to second
        SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Application::HandleMsgReally));

        return pApp->HandleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::HandleMsgReally(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Application* const pApp = reinterpret_cast<Application*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    return pApp->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Application::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{

    switch (msg)
    {
        case WM_CLOSE:
            return OnMsgWmClose(hWnd, msg, wParam, lParam);
        case WM_SIZE:
            return OnMsgSize(hWnd, msg, wParam, lParam);
        case WM_KILLFOCUS:
            return OnMsgWmKillFocus(hWnd, msg, wParam, lParam);
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: // system key like 'alt'
            return OnMsgWmKeyDownAndSysKeyDown(hWnd, msg, wParam, lParam);
        case WM_KEYUP:
        case WM_SYSKEYUP:
            return OnMsgWmKeyUpAndSysKeyUp(hWnd, msg, wParam, lParam);
        case WM_CHAR:
            return OnMsgWmChar(hWnd, msg, wParam, lParam);
        case WM_MOUSEMOVE:
            return OnMsgWmMouseMove(hWnd, msg, wParam, lParam);
        case WM_LBUTTONDOWN:
            return OnMsgWmLButtonDown(hWnd, msg, wParam, lParam);
        case WM_LBUTTONUP:
            return OnMsgWmLButtonUp(hWnd, msg, wParam, lParam);
        case WM_MBUTTONDOWN:
            return OnMsgWmMButtonDown(hWnd, msg, wParam, lParam);
        case WM_MBUTTONUP:
            return OnMsgWmMButtonUp(hWnd, msg, wParam, lParam);
        case WM_RBUTTONDOWN:
            return OnMsgWmRButtonDown(hWnd, msg, wParam, lParam);
        case WM_RBUTTONUP:
            return OnMsgWmRButtonUp(hWnd, msg, wParam, lParam);
        case WM_MOUSEWHEEL:
            return OnMsgWmMouseWheel(hWnd, msg, wParam, lParam);
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

LRESULT Application::OnMsgWmClose(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT Application::OnMsgWmKillFocus(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // if key down raises other window, then key up will not send here, so here must clear states
    _keyboard.clearState();

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    _width = LOWORD(lParam);
    _height = HIWORD(lParam);

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmKeyDownAndSysKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // if first press down, lParam & 0x40000000 = 1
    if (!(lParam & 0x40000000) || _keyboard.isAutoRepeatEnabled())
        _keyboard.onKeyPressed(static_cast<unsigned char>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmKeyUpAndSysKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    _keyboard.onKeyReleased(static_cast<unsigned char>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    _keyboard.onCharW(static_cast<wchar_t>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);

    if (pt.x >= 0 && pt.x < _window.GetWidth() && pt.y > 0 && pt.y < _window.GetHeight())
    {
        _mouse.onMouseMove(pt.x, pt.y);
        if (!_mouse.isInWindow())
        {
            SetCapture(_window.GetHWnd());
            _mouse.onMouseEnter();
        }
    } else
    {
        // 按住的时候离开窗口，不许离开
        // do not allow mouse leave this window when pressing
        if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
        {
            _mouse.onMouseMove(pt.x, pt.y);
        } else
        {
            ReleaseCapture();
            _mouse.onMouseLeave();
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onLeftMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onLeftMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onMiddleMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onMiddleMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onRightMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.onRightMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnWheelDelta(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

