
#pragma once

#include <vector>

#include "ImGui/ImGuiRenderDelegate.h"
#include "Def/DataDef.h"
#include "imgui.h"

class UiLogic : public ImGuiRenderDelegate
{
public:
    UiLogic();

public:
    void Update() override;

private:
    std::wstring GetConfigPath() const;
    void UpdateVsCodePath();
    void InitConfig();
    void WriteConfig();

private:
    ImFont* _bigFont = nullptr;
    std::wstring _vsCodePathString;
    std::vector<Folder> _allFolder;
};