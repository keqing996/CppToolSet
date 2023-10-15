
#pragma once

#include "ImGui/ImGuiRenderDelegate.h"

class UiLogic : public ImGuiRenderDelegate
{
public:
    UiLogic();

public:
    void Update() const override;

private:
    void InitConfig();

private:
    std::wstring _vsCodePathString;

};