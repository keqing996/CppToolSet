#include "ImGuiLogic.h"
#include "Window/WinWindow.h"

namespace UI
{
    ImGuiLogic::ImGuiLogic(Win32Window* topWindow)
        : _pTopWindow(topWindow)
    {
        _pTopWindow->AddImGuiLogicUpdater(this);

        ImGuiIO& io = ImGui::GetIO();
        float dpiScale = _pTopWindow->GetDpiScale();
        ImGui::GetStyle().ScaleAllSizes(dpiScale);

        _pFontNormalMsYaHei = io.Fonts->AddFontFromFileTTF(
                "c:\\Windows\\Fonts\\msyhl.ttc",
                dpiScale * NORMAL_FONT_SIZE,
                nullptr,
                io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

        _pFontLargeMsYaHei = io.Fonts->AddFontFromFileTTF(
                "c:\\Windows\\Fonts\\msyhl.ttc",
                dpiScale * LARGE_FONT_SIZE,
                nullptr,
                io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    }

    ImGuiLogic::~ImGuiLogic()
    {
        _pTopWindow->RemoveImGuiLogicUpdater(this);
    }

    void ImGuiLogic::Update()
    {
        static float f = 0.0f;
        static int counter = 0;
        static bool show_demo_window = false;
        static bool show_another_window = false;
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::PushFont(_pFontLargeMsYaHei);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::PopFont();
        ImGui::End();
    }


}