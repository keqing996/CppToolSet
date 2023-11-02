
#pragma once

#include <vector>

#include "ImGuiLogic/ImGuiLogic.h"

namespace UI
{
    class Win32Window;
}

struct Folder
{
    std::string name;
    std::string path;
};

class UiLogic : public UI::ImGuiLogic
{
public:
    explicit UiLogic(UI::Win32Window* topWindow);

public:
    void Update() override;

private:
    std::string GetConfigPath() const;
    void UpdateVsCodePath();
    void InitConfig();
    void WriteConfig();

private:
    ImFont* _bigFont = nullptr;
    std::string _vsCodePathString;
    std::vector<Folder> _allFolder;
};