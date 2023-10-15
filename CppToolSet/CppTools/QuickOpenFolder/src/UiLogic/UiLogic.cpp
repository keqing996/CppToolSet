
#include <filesystem>
#include <fstream>

#include "imgui.h"
#include "UiLogic.h"
#include "rapidjson/document.h"

using DocumentW = rapidjson::GenericDocument<rapidjson::UTF16<>>;
using ValueW = rapidjson::GenericValue<rapidjson::UTF16<>>;

UiLogic::UiLogic()
{
    InitConfig();
}

void UiLogic::Update() const
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(main_viewport->WorkSize, ImGuiCond_Always);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Quick Open Folder", nullptr, window_flags);


    ImGui::End();
}

std::wstring UiLogic::GetConfigPath() const
{
    auto currentPath = std::filesystem::current_path();
    auto configPath = currentPath /= L"quick_open_folder.json";
    return configPath;
}

void UiLogic::InitConfig()
{
    auto configPath = GetConfigPath();

    if (!std::filesystem::exists(configPath))
    {
        // create file
        std::wofstream fs(configPath, std::ios::out);
        fs.close();
    }

    // read file content
    std::wifstream inputFile(configPath);
    std::wstringstream fileContent;
    fileContent << inputFile.rdbuf();
    inputFile.close();

    // read form json
    DocumentW doc;
    rapidjson::ParseResult parseOk = doc.Parse(fileContent.str().c_str());
    if (parseOk)
    {
        if (doc.HasMember(L"VsCodePath") && doc[L"VsCodePath"].IsString())
            _vsCodePathString = doc[L"VsCodePath"].GetString();
        else
            _vsCodePathString = L"";

        if (doc.HasMember(L"FolderArray") && doc[L"FolderArray"].IsArray())
        {
            for (const auto& singleFolder: doc[L"FolderArray"].GetArray())
            {
                if (singleFolder.HasMember(L"Name") && singleFolder[L"Name"].IsString()
                    && singleFolder.HasMember(L"Path") && singleFolder[L"Path"].IsString())
                {
                    _allFolder.emplace_back(
                            singleFolder[L"Name"].GetString(),
                            singleFolder[L"Path"].GetString()
                    );
                }
            }
        }
    }
    else
    {
        _vsCodePathString = L"";
    }
}

void UiLogic::WriteConfig()
{
    auto configPath = GetConfigPath();
    std::wofstream fs(configPath, std::ios::out);
    if (!fs.is_open())
        return;


}
