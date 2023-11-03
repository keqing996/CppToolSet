
#include <filesystem>
#include <fstream>

#include "Framework/Window/WinWindow.h"
#include "StringUtil.hpp"
#include "WinApi/WinApiFileDialog.h"
#include "MainLogic.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "imgui.h"

using RapidJsonDoc = rapidjson::GenericDocument<rapidjson::UTF8<>>;
using RapidJsonValue = rapidjson::GenericValue<rapidjson::UTF8<>>;
using RapidJsonStringBuffer = rapidjson::GenericStringBuffer<rapidjson::UTF8<>>;
using RapidJsonPrettyWriter = rapidjson::PrettyWriter<RapidJsonStringBuffer, rapidjson::UTF8<>, rapidjson::UTF8<>>;

MainLogic::MainLogic()
{
    InitConfig();
}

void MainLogic::Update()
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
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 6});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);

    UpdateFolderPath();

    ImGui::Dummy(ImVec2 {0, 35});

    UpdateVsCodePath();

    ImGui::PopStyleVar(2);
    ImGui::End();
}

void MainLogic::UpdateVsCodePath()
{
    UpdateTitle("VS Code Path");

    if (ImGui::Button("路径", {50, 0}))
    {
        auto newVsCodePath = WinApi::FileDialog::OpenFile(L"Choose VS Code Path");
        if (newVsCodePath.has_value())
        {
            _vsCodePathString = Util::StringConvert::WideStringToString(newVsCodePath.value());
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

void MainLogic::UpdateFolderPath()
{
    UpdateTitle("Folders");

    for (const auto& folder : _allFolder)
        UpdateSingleFolder(folder);

    if (ImGui::Button("Add"))
    {
        auto newFolder = WinApi::FileDialog::OpenDirectory(L"Choose Folder");
        if (newFolder.has_value())
        {
            _allFolder.emplace_back("NewFolder", Util::StringConvert::WideStringToString(newFolder.value()));
            WriteConfig();
        }
    }
}

void MainLogic::UpdateSingleFolder(const Folder& folder)
{
    ImGuiStyle& style = ImGui::GetStyle();
    auto childHeight = (ImGui::CalcTextSize("I").y + style.FramePadding.y * 2) * 2 + 3 * style.ItemSpacing.y;

    ImGui::BeginChild(folder.name.c_str(), ImVec2 {0, childHeight}, true);

    auto buttonWidth = (ImGui::GetContentRegionAvail().x - (style.ItemSpacing.x * 3)) / 4;

    // name, path
    ImGui::Text("Name: ");

    ImGui::SameLine();

    ImGui::Text("...");

    // function button

    ImGui::Button("Open", ImVec2{buttonWidth, 0});
    ImGui::SameLine();
    ImGui::Button("OpenWithCode", ImVec2{buttonWidth, 0});
    ImGui::SameLine();
    ImGui::Button("Delete", ImVec2{buttonWidth, 0});
    ImGui::SameLine();
    ImGui::Button("Rename", ImVec2{buttonWidth, 0});

    ImGui::EndChild();
}

void MainLogic::UpdateTitle(const char* title)
{
    ImGui::PushFont(_pTopWindow->GetRender()->GetLargeFont());
    {
        auto titleWidth = ImGui::CalcTextSize(title);

        ImGui::SetCursorPosX(0.5 * (ImGui::GetWindowWidth() - titleWidth.x));
        ImGui::Text("%s", title);
    }
    ImGui::PopFont();
}

std::string MainLogic::GetConfigPath() const
{
    auto currentPath = std::filesystem::current_path();
    auto configPath = currentPath /= "quick_open_folder.json";
    return configPath.string();
}

void MainLogic::InitConfig()
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
    RapidJsonDoc doc;
    rapidjson::ParseResult parseOk = doc.Parse(fileContent.str().c_str());
    if (parseOk)
    {
        if (doc.HasMember("VsCodePath") && doc["VsCodePath"].IsString())
            _vsCodePathString = doc["VsCodePath"].GetString();
        else
            _vsCodePathString = "";

        if (doc.HasMember("FolderArray") && doc["FolderArray"].IsArray())
        {
            for (const auto& singleFolder: doc["FolderArray"].GetArray())
            {
                if (singleFolder.HasMember("Name") && singleFolder["Name"].IsString()
                    && singleFolder.HasMember("Path") && singleFolder["Path"].IsString())
                {
                    _allFolder.emplace_back(
                            singleFolder["Name"].GetString(),
                            singleFolder["Path"].GetString()
                    );
                }
            }
        }
    }
    else
    {
        _vsCodePathString = "";
    }
}

void MainLogic::WriteConfig()
{
    if (_vsCodePathString.length() == 0 && _allFolder.empty())
        return;

    auto configPath = GetConfigPath();
    std::wofstream fs(configPath, std::ios::out);
    if (!fs.is_open())
        return;

    RapidJsonDoc doc;

    doc.SetObject();

    if (_vsCodePathString.length() > 0)
    {
        RapidJsonValue value(_vsCodePathString.c_str(), _vsCodePathString.length());
        doc.AddMember("VsCodePath", value, doc.GetAllocator());
    }

    if (!_allFolder.empty())
    {
        RapidJsonValue arr(rapidjson::kArrayType);

        for (const auto& f : _allFolder)
        {
            RapidJsonValue folderObj(rapidjson::kObjectType);

            RapidJsonValue name(f.name.c_str(), f.name.length());
            RapidJsonValue path(f.path.c_str(), f.path.length());

            folderObj.AddMember("Name", name, doc.GetAllocator());
            folderObj.AddMember("Path", path, doc.GetAllocator());

            arr.PushBack(folderObj, doc.GetAllocator());
        }

        doc.AddMember("FolderArray", arr, doc.GetAllocator());
    }

    RapidJsonStringBuffer sb;
    RapidJsonPrettyWriter writer {sb};

    doc.Accept(writer);

    fs << sb.GetString();

    fs.close();
}
