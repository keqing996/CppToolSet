
#pragma once

#include <vector>

#include "Framework/ImGuiLogic/ImGuiLogic.h"

struct Folder
{
    std::string name;
    std::string path;
};

class MainLogic : public UiTemplate::ImGuiLogic
{
public:
    MainLogic();

public:
    void Update() override;

private:
    std::string GetConfigPath() const;
    void UpdateVsCodePath();
    void InitConfig();
    void WriteConfig();

private:
    std::string _vsCodePathString;
    std::vector<Folder> _allFolder;
};