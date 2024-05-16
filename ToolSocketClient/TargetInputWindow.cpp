#include "TargtInputWindow.h"
#include "imgui.h"
#include "Resource/Resource.h"

TargetInputWindow::TargetInputWindow()
    : _pWindow(std::make_unique<Infra::ImGuiWinApp>(400, 200, "Simple Socket", (int)Infra::WindowStyle::HaveTitleBar | (int)Infra::WindowStyle::HaveClose))
{
    _pWindow->EnableVSync(true);
    _pWindow->GetNativeWindow().SetIcon(IDI_ICON1);
    _pWindow->SetTickFunction<Infra::ImGuiWinApp::TickStage::OnFrame>([this](Infra::ImGuiWinApp& window)->void
    {
        UpdateView(window);
    });

    std::fill(_inputBuffer.begin(), _inputBuffer.end(), 0);
}

void TargetInputWindow::UpdateView(Infra::ImGuiWinApp& window)
{
    const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(mainViewport->WorkSize, ImGuiCond_Always);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::PushFont(window.GetFontBoldLarge());
    {
        ImGui::Begin("Socket", nullptr, window_flags);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {6, 12});
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 6});
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.9f, 0.32f, 0.0f, 0.2f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 {1.0f, 0.64f, 0.0f, 0.7f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 {1.0f, 0.64f, 0.0f, 1.0f});

        ImGuiStyle& style = ImGui::GetStyle();

        // Label
        {
            std::string title = "SetTargetIp";
            float size = ImGui::CalcTextSize(title.c_str()).x + style.FramePadding.x * 2.0f;
            float avail = ImGui::GetContentRegionAvail().x;

            float off = (avail - size) * 0.5f;
            if (off > 0.0f)
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

            ImGui::TextUnformatted(title.c_str());
        }

        // Input field
        {
            float size = 150 + style.FramePadding.x * 2.0f;
            float avail = ImGui::GetContentRegionAvail().x;

            float off = (avail - size) * 0.5f;
            if (off > 0.0f)
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

            ImGui::PushItemWidth(150);
            ImGui::InputText("##Ip", _inputBuffer.data(), 256);
        }

        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(3);
    }
    ImGui::PopFont();

    ImGui::End();
}

void TargetInputWindow::Loop()
{
    _pWindow->AppLoop();
}
