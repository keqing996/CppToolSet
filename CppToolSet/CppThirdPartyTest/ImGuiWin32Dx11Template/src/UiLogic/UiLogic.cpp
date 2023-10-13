
#include "imgui.h"
#include "UiLogic.h"

bool test = false;

void UiLogic::Update() const
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(main_viewport->WorkSize, ImGuiCond_Always);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_MenuBar;

    if (!ImGui::Begin("Dear ImGui Demo", nullptr, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("AAA"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("BBB"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("CCC"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("asd asd asd asd ");
    ImGui::Spacing();

    ImGui::PopItemWidth();

    ImGui::End();
}
