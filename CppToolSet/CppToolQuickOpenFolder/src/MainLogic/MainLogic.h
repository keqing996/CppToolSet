
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
    void UpdateVsCodePath();
    void UpdateFolderPath();

    void UpdateTitle(const char* name);
    void UpdateSingleFolder(const Folder& folder);

    std::string GetConfigPath() const;
    void InitConfig();
    void WriteConfig();

private:
    std::string _vsCodePathString;
    std::vector<Folder> _allFolder;
};