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

private:
    void UpdateView(Infra::ImGuiWinApp& window);

private:
    std::unique_ptr<Infra::ImGuiWinApp> _pWindow = nullptr;
    std::array<char, 256> _inputBuffer;
};


