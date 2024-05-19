#pragma once

#include <array>
#include <memory>
#include "ImGuiWinApp/ImGuiWinApp.h"

class TargetInputWindow
{
public:
    TargetInputWindow();

public:
    void Loop();
    std::string GetInputContent();

private:
    void UpdateView(Infra::ImGuiWinApp& window);
    void Connect();

private:
    std::unique_ptr<Infra::ImGuiWinApp> _pWindow = nullptr;
    std::array<char, 256> _inputBuffer;
};


