#pragma once

#include <d3d11.h>
#include "Define/WindowsPlatformMinDef.h"

namespace UI
{
    class ImGuiRender
    {
    public:
        ImGuiRender();
        ~ImGuiRender();

    public:
        void SetUp(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
        void NewFrame();
        void RenderDrawData();
    };
}