
#include <fstream>
#include <format>
#include <filesystem>
#include <Infra/String.hpp>
#include <Infra/Process.hpp>
#include <Infra/Windows/FileDialog.hpp>
#include <imgui.h>
#include "nlohmann/json.hpp"
#include "QuickOpenFolder.h"

static int _currentChangingNameIndex = -1;

QuickOpenFolder::QuickOpenFolder()
{
    InitConfig();
}

void QuickOpenFolder::Update(Infra::ImGuiWinApp& window)
{
    const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(mainViewport->WorkSize, ImGuiCond_Always);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Quick Open Folder", nullptr, window_flags);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {6, 12});
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 6});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.9f, 0.32f, 0.0f, 0.2f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 {1.0f, 0.64f, 0.0f, 0.7f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 {1.0f, 0.64f, 0.0f, 1.0f});

    ImGuiStyle& style = ImGui::GetStyle();
    btnWidth = (ImGui::GetContentRegionAvail().x - (style.ItemSpacing.x * 4)) / 5;

    UpdateFolderPath(window);

    ImGui::Dummy(ImVec2 {0, 35});

    UpdateVsCodePath(window);

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(3);
    ImGui::End();
}

void QuickOpenFolder::UpdateVsCodePath(Infra::ImGuiWinApp& window)
{
    UpdateTitle("VS Code Path", window);

    if (ImGui::Button((const char*)u8"路径", {btnWidth, 0}))
    {
        auto newVsCodePath = Infra::Dialog::OpenDirectory("Choose VS Code Path");
        if (newVsCodePath.has_value())
        {
            _vsCodePathString = newVsCodePath.value() + "\\";
            WriteConfig();
        }
    }

    ImGui::SameLine();

    ImGui::SetNextItemWidth(-1);

    if (_vsCodePathString.empty())
        ImGui::Text("...");
    else
        ImGui::Text("%s", _vsCodePathString.c_str());
}

void QuickOpenFolder::UpdateFolderPath(Infra::ImGuiWinApp& window)
{
    UpdateTitle("Folders", window);

    std::vector<int> goingToDeleteIndex;

    for (int i = 0; i < _allFolder.size(); i++)
        UpdateSingleFolder(i, goingToDeleteIndex, window);

    if (!goingToDeleteIndex.empty())
    {
        for (int i = goingToDeleteIndex.size() - 1; i >= 0; i--)
            _allFolder.erase(_allFolder.begin() + goingToDeleteIndex[i]);

        WriteConfig();
    }

    ImGui::Dummy(ImVec2 {0, 15});

    if (ImGui::Button("Add", {-1, 0}))
    {
        auto newFolder = Infra::Dialog::OpenDirectory("Choose Folder");
        if (newFolder.has_value())
        {
            _allFolder.emplace_back("NewFolder", newFolder.value());
            WriteConfig();
        }
    }
}

void QuickOpenFolder::UpdateSingleFolder(int index, std::vector<int>& goingToDeleteIndex, Infra::ImGuiWinApp& window)
{
    auto& folder = _allFolder[index];

    // name, path
    ImGui::PushFont(window.GetFontBoldNormal());
    {
        ImGui::Text("%s", folder.name.c_str());
    }
    ImGui::PopFont();

    ImGui::SameLine();

    ImGui::Text("%s", folder.path.c_str());

    if (_currentChangingNameIndex == index)
    {
        static char nameBuffer[255];

        if (ImGui::Button("Confirm", ImVec2{btnWidth, 0}))
        {
            _currentChangingNameIndex = -1;
            folder.name = std::string {nameBuffer};
            WriteConfig();
        }

        ImGui::SameLine();

        ImGui::InputText("##", nameBuffer, sizeof(nameBuffer));
    }
    else
    {
        // function button
        std::string openBtnLabel = std::format("Open##{}", index);
        std::string cmdBtnLabel = std::format("Cmd##{}", index);
        std::string vsCodeBtnLabel = std::format("VsCode##{}", index);
        std::string deleteBtnLabel = std::format("Delete##{}", index);
        std::string renameBtnLabel = std::format("Rename##{}", index);

        if (ImGui::Button(openBtnLabel.c_str(), ImVec2{btnWidth, 0}))
        {
            Infra::Process::CreateProcessAndDetach("explorer.exe " + folder.path);
        }

        ImGui::SameLine();

        if (ImGui::Button(cmdBtnLabel.c_str(), ImVec2{btnWidth, 0}))
        {
            std::string para = "/K \"cd " + folder.path + "\"";
            Infra::Process::CreateProcessAndDetach("cmd.exe " + para);
        }

        ImGui::SameLine();

        if (ImGui::Button(vsCodeBtnLabel.c_str(), ImVec2{btnWidth, 0}))
        {
            Infra::Process::CreateProcessAndDetach(std::format("Code.exe {} {}", _vsCodePathString, folder.path));
        }

        ImGui::SameLine();

        if (ImGui::Button(deleteBtnLabel.c_str(), ImVec2{btnWidth, 0}))
        {
            goingToDeleteIndex.push_back(index);
        }

        ImGui::SameLine();

        if (ImGui::Button(renameBtnLabel.c_str(), ImVec2{btnWidth, 0}))
        {
            _currentChangingNameIndex = index;
        }
    }
}

void QuickOpenFolder::UpdateTitle(const char* title, Infra::ImGuiWinApp& window)
{
    ImGui::PushFont(window.GetFontBoldLarge());
    {
        auto titleWidth = ImGui::CalcTextSize(title);

        ImGui::SetCursorPosX(0.5 * (ImGui::GetWindowWidth() - titleWidth.x));
        ImGui::Text("%s", title);
    }
    ImGui::PopFont();
}

std::string QuickOpenFolder::GetConfigPath() const
{
    auto currentPath = std::filesystem::current_path();
    auto configPath = currentPath /= "quick_open_folder.json";
    return configPath.string();
}

void QuickOpenFolder::InitConfig()
{
    auto configPath = GetConfigPath();

    if (!std::filesystem::exists(configPath))
    {
        // create file
        std::ofstream fs(configPath, std::ios::out);
        fs.close();
    }

    // read file content
    std::ifstream inputFile(configPath);
    std::stringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    // read form json
    std::string fileContentStr = fileContent.str();
    if (fileContentStr.empty())
    {
        WriteConfig();
    }
    else
    {
        nlohmann::json doc = nlohmann::json::parse(fileContentStr);
        if (doc.contains("VsCodePath") && doc["VsCodePath"].is_string())
            _vsCodePathString = doc["VsCodePath"];
        else
            _vsCodePathString = "";

        if (doc.contains("FolderArray") && doc["FolderArray"].is_array())
        {
            for (const auto& singleFolder: doc["FolderArray"])
            {
                if (singleFolder.contains("Name") && singleFolder["Name"].is_string()
                    && singleFolder.contains("Path") && singleFolder["Path"].is_string())
                {
                    _allFolder.emplace_back(singleFolder["Name"],singleFolder["Path"]);
                }
            }
        }
    }
}

void QuickOpenFolder::WriteConfig()
{
    if (_vsCodePathString.empty() && _allFolder.empty())
        return;

    auto configPath = GetConfigPath();
    std::ofstream fs(configPath, std::ios::out);
    if (!fs.is_open())
        return;

    nlohmann::json doc;

    if (!_vsCodePathString.empty())
        doc["VsCodePath"] = _vsCodePathString;

    if (!_allFolder.empty())
    {
        nlohmann::json arr = nlohmann::json::array();

        for (const auto& [name, path] : _allFolder)
        {
            nlohmann::json folderObj;
            folderObj["Name"] = name;
            folderObj["Path"] = path;

            arr.push_back(folderObj);
        }

        doc["FolderArray"] = arr;
    }

    fs << std::setw(4) << doc << std::endl;

    fs.close();
}