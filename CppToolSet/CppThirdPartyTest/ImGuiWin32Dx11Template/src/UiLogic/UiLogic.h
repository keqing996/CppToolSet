
#pragma once

#include "ImGui/ImGuiRenderDelegate.h"

class UiLogic : public ImGuiRenderDelegate
{
public:
    void Update() const override;
};