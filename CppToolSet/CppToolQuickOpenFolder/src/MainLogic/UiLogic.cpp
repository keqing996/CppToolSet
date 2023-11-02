
#include <filesystem>
#include <fstream>

#include "imgui_impl_win32.h"
#include "UiLogic.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

using RapidJsonDoc = rapidjson::GenericDocument<rapidjson::UTF8<>>;
using RapidJsonValue = rapidjson::GenericValue<rapidjson::UTF8<>>;
using RapidJsonStringBuffer = rapidjson::GenericStringBuffer<rapidjson::UTF8<>>;
using RapidJsonPrettyWriter = rapidjson::PrettyWriter<RapidJsonStringBuffer, rapidjson::UTF8<>, rapidjson::UTF8<>>;

UiLogic::UiLogic(UI::Win32Window* topWindow) : UI::ImGuiLogic(topWindow)
{
    InitConfig();
}

void UiLogic::Update()
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
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

    UpdateVsCodePath();

    ImGui::PopStyleVar(2);
    ImGui::End();
}

void UiLogic::UpdateVsCodePath()
{
    static const char* title = "VS Code Path";

    ImGui::PushFont(_bigFont);
    {
        auto titleWidth = ImGui::CalcTextSize(title);

        ImGui::SetCursorPosX(0.5 * (ImGui::GetWindowWidth() - titleWidth.x));
        ImGui::Text("%s", title);
    }
    ImGui::PopFont();

    ImGui::Button("...", {45, 0});
    ImGui::SameLine();

    //static char buf[128] = "click on a button to set focus";

    ImGui::SetNextItemWidth(-1);

    if (_vsCodePathString.empty())
        ImGui::Text("中文");
    else
        ImGui::Text("asdasds");
    //ImGui::InputText("##", _vsCodePathString.c_str(), 0, ImGuiInputTextFlags_ReadOnly);
}

std::string UiLogic::GetConfigPath() const
{
    auto currentPath = std::filesystem::current_path();
    auto configPath = currentPath /= "quick_open_folder.json";
    return configPath.string();
}

void UiLogic::InitConfig()
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

void UiLogic::WriteConfig()
{
    if (_vsCodePathString.length() == 0 && _allFolder.empty())
        return;

    auto configPath = GetConfigPath();
    std::wofstream fs(configPath, std::ios::out);
    if (!fs.is_open())
        return;

    RapidJsonDoc doc;

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

    fs << doc.GetString();

    fs.close();
}
