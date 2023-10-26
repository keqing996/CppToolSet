
#include "Application.h"
#include "RendererHardwareInterface/OpenGL/RhiOpenGL.h"

Application::~Application()
{
    DestroyEditor();
    DestroyRenderer();
    DestroyWindow();
}

void Application::InitWindow(int windowWidth, int windowHeight, const wchar_t* name)
{
    _width = windowWidth;
    _height = windowHeight;

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

void Application::DestroyWindow()
{
    ::DestroyWindow(_hWnd);
}

void Application::SetupRenderer(Renderer::RendererApi api)
{
    _pRender = Renderer::Renderer::Create(api);
    _pRender->SetUp();

    _pEditor = new Editor::Editor(api);
    _pEditor->SetUp();
}

void Application::DestroyRenderer()
{
    if (_pRender != nullptr)
        _pRender->Destroy();

    delete _pRender;
}

void Application::DestroyEditor()
{
    if (_pEditor != nullptr)
        _pEditor->Destroy();

    delete _pEditor;
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

            _pRender->Render();
        }

        if (shouldStop)
            break;
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

HWND Application::GetWindowHandle() const
{
    return _hWnd;
}

Input::Keyboard::Accessor Application::GetKeyboardAccessor()
{
    return _keyboard.GetAccessor();
}

Input::Mouse::Accessor Application::GetMouseAccessor()
{
    return _mouse.GetAccessor();
}

const Renderer::Renderer* Application::GetRenderer() const
{
    return _pRender;
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
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
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
    _keyboard.ClearState();

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
    if (!(lParam & 0x40000000) || _keyboard.GetAccessor().IsAutoRepeatEnabled())
        _keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmKeyUpAndSysKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    _keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmChar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    _keyboard.OnCharW(static_cast<wchar_t>(wParam));

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);

    if (pt.x >= 0 && pt.x < _width && pt.y > 0 && pt.y < _height)
    {
        _mouse.OnMouseMove(pt.x, pt.y);
        if (!_mouse.GetAccessor().IsInWindow())
        {
            SetCapture(_hWnd);
            _mouse.OnMouseEnter();
        }
    } else
    {
        // 按住的时候离开窗口，不许离开
        // do not allow mouse leave this window when pressing
        if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
        {
            _mouse.OnMouseMove(pt.x, pt.y);
        } else
        {
            ReleaseCapture();
            _mouse.OnMouseLeave();
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmLButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnLeftMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmLButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnLeftMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmMButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnMiddleMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmMButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnMiddleMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmRButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnRightMousePressed(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnRightMouseReleased(pt.x, pt.y);
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::OnMsgWmMouseWheel(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    POINTS pt = MAKEPOINTS(lParam);
    _mouse.OnWheelDelta(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma endregion


