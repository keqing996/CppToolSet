#include "EditorBackend.h"
#include "Application/Application.h"
#include "OpenGL/EditorBackendOpenGL.h"
#include "imgui.h"
#include "imgui_impl_win32.h"

namespace Editor
{
    void EditorBackend::SetUp()
    {
        // Create ImGui Context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsLight();

        // Setup ImGui Render Backend
        SetUpBackend();

        // Get Dpi Scale
        auto hWnd = Application::GetInstance()->GetWindowHandle();
        float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(hWnd);
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

        // Load Font
        float fontSize = 16 * dpiScale;
        ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\consola.ttf", fontSize, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
        IM_ASSERT(font != nullptr);
    }

    EditorBackend* EditorBackend::Create(Renderer::RendererApi api)
    {
        switch (api)
        {
            case Renderer::RendererApi::OpenGL:
                return new EditorBackendOpenGL();
            case Renderer::RendererApi::Vulkan:
                break;
            case Renderer::RendererApi::D3D11:
                break;
            case Renderer::RendererApi::D3D12:
                break;
        }

        return nullptr;
    }
}