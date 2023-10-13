
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
    ID3D11Device* _pD3dDevice = nullptr;
    ID3D11DeviceContext* _pD3dDeviceContext = nullptr;
    IDXGISwapChain* _pSwapChain = nullptr;
    ID3D11RenderTargetView* _pMainRenderTargetView = nullptr;
};