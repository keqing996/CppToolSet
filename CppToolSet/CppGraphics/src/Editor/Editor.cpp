#include "Editor.h"
#include "Application/Application.h"
#include "imgui.h"
#include "imgui_impl_win32.h"

namespace Editor
{
    Editor::Editor(RendererApi api)
    {
        _pBackend = EditorBackend::Create(api);
    }

    void Editor::SetUp()
    {
        // Create ImGui Context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsLight();

        // Backend Setup
        _pBackend->SetUp();

        // Get Dpi Scale
        auto hWnd = Application::GetInstance()->GetWindowHandle();
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(hWnd);
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

        // Load Font
        float fontSize = 16 * dpiScale;
        ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\consola.ttf", fontSize, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        IM_ASSERT(font != nullptr);
    }

    void Editor::Destroy()
    {
        _pBackend->Destroy();

        ImGui::DestroyContext();

        delete _pBackend;
    }

    void Editor::Update()
    {
        _pBackend->BeforeUpdate();

        ImGui::NewFrame();

        // temp
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(main_viewport->WorkPos.x / 3, main_viewport->WorkPos.y / 2), ImGuiCond_Always);

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;

        ImGui::Begin("Test", nullptr, window_flags);

        ImGui::Button("Test");

        ImGui::End();



        ImGui::Render();

        _pBackend->AfterUpdate();
    }
}