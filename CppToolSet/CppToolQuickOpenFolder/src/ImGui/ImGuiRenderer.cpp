
#include "ImGuiRenderer.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

// win32 handler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool ImGuiRenderer::D3D11CreateDevice(HWND hWnd, unsigned int width, unsigned int height)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[] ={D3D_FEATURE_LEVEL_11_0,};

    HRESULT d3dInitResult = ::D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            featureLevelArray,
            1,
            D3D11_SDK_VERSION,
            &sd,
            &_pSwapChain,
            &_pD3dDevice,
            &featureLevel,
            &_pD3dDeviceContext);

    if (d3dInitResult != S_OK)
        return false;

    D3D11CreateRenderTarget();

    return true;
}

void ImGuiRenderer::D3D11CleanUpDevice()
{
    D3D11ClearRenderTarget();

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

void ImGuiRenderer::D3D11CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    _pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    _pD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pMainRenderTargetView);
    pBackBuffer->Release();
}

void ImGuiRenderer::D3D11ClearRenderTarget()
{
    if (_pMainRenderTargetView)
    {
        _pMainRenderTargetView->Release();
        _pMainRenderTargetView = nullptr;
    }
}

void ImGuiRenderer::D3D11OnResize(unsigned int width, unsigned int height)
{
    D3D11ClearRenderTarget();
    _pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    D3D11CreateRenderTarget();
}

void ImGuiRenderer::ImGuiCreateContext(HANDLE hWnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(_pD3dDevice, _pD3dDeviceContext);

    // Get Dpi Scale
    float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(hWnd);
    ImGui::GetStyle().ScaleAllSizes(dpiScale);

    // Load Font
    float fontSize = 16 * dpiScale;
    ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyhl.ttc", fontSize, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    IM_ASSERT(font != nullptr);
}

void ImGuiRenderer::ImGuiRenderLoop()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    for (auto p : _loopDelegate)
    {
        if (p != nullptr)
            p->Update();
    }

    ImGui::Render();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const float clear_color_with_alpha[4] = {
            clear_color.x * clear_color.w,
            clear_color.y * clear_color.w,
            clear_color.z * clear_color.w,
            clear_color.w };

    _pD3dDeviceContext->OMSetRenderTargets(1, &_pMainRenderTargetView, nullptr);
    _pD3dDeviceContext->ClearRenderTargetView(_pMainRenderTargetView, clear_color_with_alpha);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    _pSwapChain->Present(1, 0); // Present with vsync
}

LRESULT ImGuiRenderer::ImGuiHandleWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return false;
}

void ImGuiRenderer::ImGuiCleanUp()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiRenderer::AddRenderDelegate(ImGuiRenderDelegate* pRender)
{
    _loopDelegate.push_back(pRender);
}
