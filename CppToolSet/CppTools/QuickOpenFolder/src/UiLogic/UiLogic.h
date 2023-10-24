
#pragma once

#include <vector>

#include "ImGui/ImGuiRenderDelegate.h"
#include "Def/DataDef.h"

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
    std::wstring _vsCodePathString;
    std::vector<Folder> _allFolder;
};