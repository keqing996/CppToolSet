
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

void UiLogic::InitConfig()
{
    auto currentPath = std::filesystem::current_path();
    auto configPath = currentPath /= L"quick_open_folder.json";

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

    //
    DocumentW doc;
    rapidjson::ParseResult parseOk = doc.Parse(fileContent.str().c_str());
    if (parseOk)
    {

    }
}
