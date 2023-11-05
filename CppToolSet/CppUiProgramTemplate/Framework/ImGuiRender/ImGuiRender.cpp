
#include "WinApi/WinApiResource.h"

#include "ImGuiRender.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "../Resource/FontResource.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace UiTemplate
{
    void ImGuiRender::InitSharedFontAtlas()
    {
        if (_pSharedImGuiFonts != nullptr)
            return;

        _pSharedImGuiFonts = new ImFontAtlas();
    }

    ImGuiRender::ImGuiRender()
    {
    }

    ImGuiRender::~ImGuiRender()
    {
    }

    void ImGuiRender::SetUp(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
    {
        IMGUI_CHECKVERSION();

        InitSharedFontAtlas();

        ImGui::CreateContext(_pSharedImGuiFonts);
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        // close imGui ini file generate
        io.IniFilename = nullptr;

        // color light
        ImGui::StyleColorsLight();

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(pDevice, pDeviceContext);

        // Scale
        float dpiScale = GetDpiScale(reinterpret_cast<int64_t>(hWnd));
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

        // Font
        auto LoadMemoryFont = [this, dpiScale](int resId) -> ImFont*
        {
            auto defaultFontRes = WinApi::Resource::LoadResource<WinApi::Resource::DataResource>(resId);
            if (!defaultFontRes.has_value())
                return nullptr;

            return _pSharedImGuiFonts->AddFontFromMemoryTTF(
                    defaultFontRes.value().data,
                    defaultFontRes.value().size,
                    dpiScale * NORMAL_FONT_SIZE,
                    nullptr,
                    _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
        };

        auto LoadMemoryFromFile = [this, dpiScale](const char* fileName) -> ImFont*
        {
            return _pSharedImGuiFonts->AddFontFromFileTTF(
                    fileName,
                    dpiScale * NORMAL_FONT_SIZE,
                    nullptr,
                    _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
        };

#ifdef EMBEDDED_TTF

        _pFontRegular = LoadMemoryFont(RES_TTF_REGULAR);
        _pFontBold = LoadMemoryFont(RES_TTF_BOLD);

#endif

        if (_pFontRegular == nullptr)
            _pFontRegular = LoadMemoryFromFile("c:\\Windows\\Fonts\\msyhl.ttc");

        if (_pFontBold == nullptr)
            _pFontBold = LoadMemoryFromFile("c:\\Windows\\Fonts\\msyhbd.ttc");

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

    void ImGuiRender::OnWinMsg(int64_t hWnd, uint32_t msg, int64_t wParam, int64_t lParam)
    {
        ImGui_ImplWin32_WndProcHandler(
                reinterpret_cast<HWND>(hWnd),
                static_cast<UINT>(msg),
                static_cast<WPARAM>(wParam),
                static_cast<LPARAM>(lParam));
    }

    float ImGuiRender::GetDpiScale(int64_t hWnd) const
    {
        return ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));
    }

    ImFont* ImGuiRender::GetRegularFont() const
    {
        return _pFontRegular;
    }

    ImFont* ImGuiRender::GetBoldFont() const
    {
        return _pFontBold;
    }
}