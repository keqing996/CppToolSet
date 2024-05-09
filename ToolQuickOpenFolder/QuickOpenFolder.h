#pragma once

#include <string>
#include <ImGuiWinApp/ImGuiWinApp.h>

struct Folder
{
    std::string name;
    std::string path;
};

class QuickOpenFolder
{
public:
    QuickOpenFolder();

public:
    void Update(Infra::ImGuiWinApp& window);

private:
    std::string GetConfigPath() const;
    void InitConfig();
    void WriteConfig();
    void UpdateVsCodePath(Infra::ImGuiWinApp& window);
    void UpdateFolderPath(Infra::ImGuiWinApp& window);
    void UpdateSingleFolder(int index, std::vector<int>& goingToDeleteIndex, Infra::ImGuiWinApp& window);
    void UpdateTitle(const char* title, Infra::ImGuiWinApp& window);

private:
    std::string _vsCodePathString;
    std::vector<Folder> _allFolder;

    float btnWidth = 45;
};