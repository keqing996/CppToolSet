#include <imgui.h>
#include <functional>
#include "TargtInputWindow.h"
#include "Resource/Resource.h"

using namespace NWA;
using namespace IMWinApp;
using namespace Infra;

static bool SocketValid(const Socket::SocketHandle& handle)
{
    return handle.handle != nullptr;
}

TargetInputWindow::TargetInputWindow()
    : _pWindow(std::make_unique<ImGuiWinApp>(400, 200, "Simple Socket", (int)WindowStyle::HaveTitleBar | (int)WindowStyle::HaveClose))
{
    _pWindow->EnableVSync(true);
    _pWindow->GetNativeWindow().SetIcon(IDI_ICON1);
    _pWindow->SetTickFunction<ImGuiWinApp::TickStage::OnFrame>([this](ImGuiWinApp& window)->void
    {
        UpdateView(window);
    });

    std::fill(_inputBuffer.begin(), _inputBuffer.end(), 0);

    _socketHandle.handle = nullptr;
}

void AlignCenter(float itemWidth, const std::function<void()> imGuiDraw)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = itemWidth + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * 0.5f;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    imGuiDraw();
}

void TargetInputWindow::UpdateView(ImGuiWinApp& window)
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

        // Space
        ImGui::Spacing();

        // Label
        const std::string title = "SetTargetIp";
        AlignCenter(ImGui::CalcTextSize(title.c_str()).x, [&title]()-> void
        {
            ImGui::TextUnformatted(title.c_str());
        });

        // Input field
        float inputFieldSize = 150;
        AlignCenter(inputFieldSize, [&inputFieldSize, this]()-> void
        {
            ImGui::PushItemWidth(inputFieldSize);
            ImGui::InputText("##Ip", _inputBuffer.data(), 256);
        });

        // Button
        float buttonSize = 80 ;
        AlignCenter(buttonSize, [&buttonSize, this]()-> void
        {
            ImGui::PushItemWidth(buttonSize);
            if (ImGui::Button("Connect"))
                Connect();
        });

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

std::string TargetInputWindow::GetInputContent()
{
    return std::string(_inputBuffer.data());
}

void TargetInputWindow::Connect()
{
    std::string ip = GetInputContent();
    auto optSocketHandle = Socket::Create(Socket::AddressFamily::IpV4, Socket::Protocol::Tcp);
    if (optSocketHandle.has_value())
        _socketHandle = optSocketHandle.value();

    Socket::Connect(_socketHandle, )

}
