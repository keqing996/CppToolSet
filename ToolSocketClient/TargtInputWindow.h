#pragma once

#include <array>
#include <memory>
#include "ImGuiWinAppFramework/ImGuiWinApp.h"
#include "Infra/Socket.hpp"

class TargetInputWindow
{
public:
    TargetInputWindow();

public:
    void Loop();
    std::string GetInputContent();

private:
    void UpdateView(IMWinApp::ImGuiWinApp& window);
    void Connect();

private:
    std::unique_ptr<IMWinApp::ImGuiWinApp> _pWindow = nullptr;
    std::array<char, 256> _inputBuffer;
    Infra::Socket::SocketHandle _socketHandle {};
};


