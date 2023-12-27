
#include "Framework/ImGuiLogic/ImGuiLogic.h"
#include "Logic.h"

Logic::Logic() : _port(6666)
{

}

void Logic::Update()
{
    const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(mainViewport->WorkSize, ImGuiCond_Always);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Socket", nullptr, window_flags);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {6, 12});
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 6});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.9f, 0.32f, 0.0f, 0.2f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 {1.0f, 0.64f, 0.0f, 0.7f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 {1.0f, 0.64f, 0.0f, 1.0f});

    // ...

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(3);

    ImGui::End();
}

void Logic::Connect()
{

}

