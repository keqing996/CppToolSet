#include "ImGuiRender.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "Resource/Resource.h"
#include "Resource/WinResourceLoader.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace UI
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

        ImGui::StyleColorsLight();

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(pDevice, pDeviceContext);

        // Scale
        float dpiScale = GetDpiScale(reinterpret_cast<int64>(hWnd));
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

        // Font
        auto defaultFontRes = WinResourceLoader::LoadDataResource(IDR_TTF1);
        if (defaultFontRes.has_value())
        {
            _pFontNormalJetbrainsMonoMsYaHei = _pSharedImGuiFonts->AddFontFromMemoryTTF(
                    defaultFontRes.value().data,
                    defaultFontRes.value().size,
                    dpiScale * NORMAL_FONT_SIZE,
                    nullptr,
                    _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());

            _pFontLargeJetbrainsMonoMsYaHei = _pSharedImGuiFonts->AddFontFromMemoryTTF(
                    defaultFontRes.value().data,
                    defaultFontRes.value().size,
                    dpiScale * LARGE_FONT_SIZE,
                    nullptr,
                    _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
        }

        if (_pFontNormalJetbrainsMonoMsYaHei == nullptr)
        {
            _pFontNormalJetbrainsMonoMsYaHei = _pSharedImGuiFonts->AddFontFromFileTTF(
                    "c:\\Windows\\Fonts\\msyhl.ttc",
                    dpiScale * NORMAL_FONT_SIZE,
                    nullptr,
                    _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
        }

        if (_pFontLargeJetbrainsMonoMsYaHei == nullptr)
        {
            _pFontLargeJetbrainsMonoMsYaHei = _pSharedImGuiFonts->AddFontFromFileTTF(
                    "c:\\Windows\\Fonts\\msyhl.ttc",
                    dpiScale * LARGE_FONT_SIZE,
                    nullptr,
                    _pSharedImGuiFonts->GetGlyphRangesChineseSimplifiedCommon());
        }
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

    float ImGuiRender::GetDpiScale(int64 hWnd) const
    {
        return ImGui_ImplWin32_GetDpiScaleForHwnd(reinterpret_cast<HWND>(hWnd));
    }

    ImFont* ImGuiRender::GetNormalFont() const
    {
        return _pFontNormalJetbrainsMonoMsYaHei;
    }

    ImFont* ImGuiRender::GetLargeFont() const
    {
        return _pFontLargeJetbrainsMonoMsYaHei;
    }
}