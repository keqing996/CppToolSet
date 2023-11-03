#include "ImGuiLogic.h"
#include "Framework/Window/WinWindow.h"

namespace UiTemplate
{
    ImGuiLogic::ImGuiLogic(Win32Window* topWindow)
        : _pTopWindow(topWindow)
    {
        _pTopWindow->AddImGuiLogicUpdater(this);
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

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(main_viewport->WorkSize, ImGuiCond_Always);

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;

        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("MainWindow", nullptr, window_flags);

        ImGui::Text("This is some useful text. 中文中文测试");
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);

        if (ImGui::Button("Button"))
            counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::PushFont(_pTopWindow->GetRender()->GetLargeFont());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::PopFont();
        ImGui::End();
    }


}