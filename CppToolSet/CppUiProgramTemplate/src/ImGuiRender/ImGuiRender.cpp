#include "ImGuiRender.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace UI
{

    ImGuiRender::ImGuiRender()
    {
    }

    ImGuiRender::~ImGuiRender()
    {
    }

    void ImGuiRender::SetUp(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        ImGui::StyleColorsLight();

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(pDevice, pDeviceContext);
    }

    void ImGuiRender::Destroy()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiRender::NewFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRender::EndFrame()
    {
        ImGui::Render();
    }

    void ImGuiRender::RenderDrawData()
    {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiRender::OnWinMsg(int64 hWnd, uint32 msg, int64 wParam, int64 lParam)
    {
        ImGui_ImplWin32_WndProcHandler(
                reinterpret_cast<HWND>(hWnd),
                static_cast<UINT>(msg),
                static_cast<WPARAM>(wParam),
                static_cast<LPARAM>(lParam));
    }
}