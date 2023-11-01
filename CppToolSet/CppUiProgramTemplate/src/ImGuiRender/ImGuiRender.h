#pragma once

#include <d3d11.h>
#include "Define/WindowsPlatform.h"
#include "Window/IWinMsgReceiver.h"

namespace UI
{
    class ImGuiRender : public IWinMsgReceiver
    {
    public:
        ImGuiRender();
        ~ImGuiRender();

    public:
        void SetUp(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
        void Destroy();
        void NewFrame();
        void EndFrame();
        void RenderDrawData();

        void OnWinMsg(int64 hWnd, uint32 msg, int64 wParam, int64 lParam) override;

        float GetDpiScale(int64 hWnd) const;
    };
}