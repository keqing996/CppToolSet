
#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "Util/NonCopyable.h"

class ImGuiRenderer : public NonCopyable
{
public:
    // d3d11
    bool D3D11CreateDevice(HWND hWnd);
    void D3D11CleanUpDevice();
    void D3D11CreateRenderTarget();
    void D3D11ClearRenderTarget();
    void D3D11OnResize(unsigned int width, unsigned int height);

    // im gui
    void ImGuiCreateContext(HANDLE hWnd);
    void ImGuiRenderLoop();
    LRESULT ImGuiHandleWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void ImGuiCleanUp();

private:
    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
};