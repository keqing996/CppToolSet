#include "WinWindow.h"
#include "Define/WindowsPlatform.h"

namespace UI
{
    Win32Window::Win32Window(const char* windowRegisterName, const char* windowTitle, int width, int height)
        : _windowRegisterName(windowRegisterName)
        , _windowTitle(windowTitle)
        , _width(width)
        , _height(height)
    {
    }

    Win32Window::~Win32Window()
    {
        D3dDestroyDevice();
        Win32DestroyWindow();
        Win32UnRegisterWindow();
    }

    bool Win32Window::SetUp()
    {
        Win32RegisterWindow();
        Win32CreateWindow();

        if (D3dCreateDevice())
            return false;

        return true;
    }

    void Win32Window::Show()
    {
        ::ShowWindow(_hWnd, SW_SHOWDEFAULT);
        ::UpdateWindow(_hWnd);
    }

    void Win32Window::UpdateWinMessage(bool* isQuit)
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

    void Win32Window::ClearColor()
    {
        const float clear_color_with_alpha[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
        _pD3dDeviceContext->OMSetRenderTargets(1, &_pMainRenderTarget, nullptr);
        _pD3dDeviceContext->ClearRenderTargetView(_pMainRenderTarget, clear_color_with_alpha);
    }

    void Win32Window::SwapChain()
    {
        _pSwapChain->Present(1, 0);
    }

    HWND Win32Window::GetWindowHandle() const
    {
        return _hWnd;
    }

    ID3D11Device* Win32Window::GetD3dDevice() const
    {
        return _pD3dDevice;
    }

    ID3D11DeviceContext* Win32Window::GetD3dDeviceContext() const
    {
        return _pD3dDeviceContext;
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

        int newWidth = (int)LOWORD(lParam);
        int newHeight = (int)HIWORD(lParam);

        if (_width != newWidth || _height != newHeight)
        {
            _width = newWidth;
            _height = newHeight;

            D3dDestroyRenderTarget();
            _pSwapChain->ResizeBuffers(0, _width, _height, DXGI_FORMAT_UNKNOWN, 0);
            D3dCreateRenderTarget();
        }

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

    void Win32Window::Win32RegisterWindow()
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
                _windowRegisterName,
                nullptr
        };

        ::RegisterClassEx(&wc);
    }

    void Win32Window::Win32CreateWindow()
    {
        _hWnd = ::CreateWindow(
                _windowRegisterName,
                _windowTitle,
                WS_OVERLAPPEDWINDOW,
                100,
                100,
                _width,
                _height,
                nullptr,
                nullptr,
                GetModuleHandle(nullptr),
                this);
    }

    void Win32Window::Win32DestroyWindow()
    {
        ::DestroyWindow(_hWnd);
    }

    void Win32Window::Win32UnRegisterWindow()
    {
        ::UnregisterClass(_windowRegisterName, GetModuleHandle(nullptr));
    }

    bool Win32Window::D3dCreateDevice()
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = _hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pD3dDevice, &featureLevel, &_pD3dDeviceContext);
        if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pD3dDevice, &featureLevel, &_pD3dDeviceContext);
        if (res != S_OK)
            return false;

        D3dCreateRenderTarget();
        return true;
    }

    void Win32Window::D3dCreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        _pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        _pD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pMainRenderTarget);
        pBackBuffer->Release();
    }

    void Win32Window::D3dDestroyDevice()
    {
        D3dDestroyRenderTarget();

        if (_pSwapChain)
        {
            _pSwapChain->Release();
            _pSwapChain = nullptr;
        }

        if (_pD3dDeviceContext)
        {
            _pD3dDeviceContext->Release();
            _pD3dDeviceContext = nullptr;
        }

        if (_pD3dDevice)
        {
            _pD3dDevice->Release();
            _pD3dDevice = nullptr;
        }
    }

    void Win32Window::D3dDestroyRenderTarget()
    {
        if (_pMainRenderTarget)
        {
            _pMainRenderTarget->Release();
            _pMainRenderTarget = nullptr;
        }
    }


}