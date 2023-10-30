#include "ImGuiRender.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace UI
{

    ImGuiRender::ImGuiRender()
    {
    }

    ImGuiRender::~ImGuiRender()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
    }

    void ImGuiRender::SetUp(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    {
        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(pDevice, pDeviceContext);
    }

    void ImGuiRender::NewFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
    }

    void ImGuiRender::RenderDrawData()
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}