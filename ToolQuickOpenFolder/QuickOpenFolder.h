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
    void LoadChineseFonts(Infra::ImGuiWinApp& window);
    void Update();

private:
    std::string GetConfigPath() const;
    void InitConfig();
    void WriteConfig();
    void UpdateVsCodePath();
    void UpdateFolderPath();
    void UpdateSingleFolder(int index, std::vector<int>& goingToDeleteIndex);
    void UpdateTitle(const char* title);

private:
    std::string _vsCodePathString;
    std::vector<Folder> _allFolder;

    float btnWidth = 45;

    ImFont* _pFontRegularNormal = nullptr;
    ImFont* _pFontRegularLarge = nullptr;
    ImFont* _pFontBoldNormal = nullptr;
    ImFont* _pFontBoldLarge = nullptr;
};