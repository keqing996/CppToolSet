
#include <filesystem>
#include <fstream>

#include "imgui.h"
#include "UiLogic.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

using RapidJsonDocW = rapidjson::GenericDocument<rapidjson::UTF16<>>;
using RapidJsonValueW = rapidjson::GenericValue<rapidjson::UTF16<>>;
using RapidJsonStringBufferW = rapidjson::GenericStringBuffer<rapidjson::UTF16<>>;
using RapidJsonPrettyWriterW = rapidjson::PrettyWriter<RapidJsonStringBufferW, rapidjson::UTF16<>, rapidjson::UTF16<>>;

UiLogic::UiLogic()
{
    InitConfig();
}

void UiLogic::Update() const
{
    // helper function




    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(main_viewport->WorkSize, ImGuiCond_Always);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImVec2 buttonSize {100, 0};
    ImVec2 windowSize = ImGui::GetWindowSize();

    ImGui::Begin("Quick Open Folder", nullptr, window_flags);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {2, 6});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

    ImGui::Button("Test4", buttonSize);
    ImGui::SameLine();

    static char buf[128] = "click on a button to set focus";

    ImGui::PushItemWidth(-1);
    ImGui::InputText("##", buf, IM_ARRAYSIZE(buf));
    ImGui::PopItemWidth();

    /*
    ImVec2 currentCursor = ImGui::GetCursorPos();

    ImVec2 buttonBeginPos {
            (windowSize.x - (buttonSize.x) * 3 - (ImGui::GetStyle().ItemSpacing.x) * 2) / 2,
            currentCursor.y
        };

    ImGui::SetCursorPos(buttonBeginPos);

    ImGui::Button("Test1", buttonSize);
    ImGui::SameLine();
    ImGui::Button("Test2", buttonSize);
    ImGui::SameLine();
    ImGui::Button("Test3", buttonSize);

    ImGui::Button("Test4", buttonSize);
*/
    ImGui::PopStyleVar(2);
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
    RapidJsonDocW doc;
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
    if (_vsCodePathString.length() == 0 && _allFolder.empty())
        return;

    auto configPath = GetConfigPath();
    std::wofstream fs(configPath, std::ios::out);
    if (!fs.is_open())
        return;

    RapidJsonDocW doc;

    if (_vsCodePathString.length() > 0)
    {
        RapidJsonValueW value(_vsCodePathString.c_str(), _vsCodePathString.length());
        doc.AddMember(L"VsCodePath", value, doc.GetAllocator());
    }

    if (!_allFolder.empty())
    {
        RapidJsonValueW arr(rapidjson::kArrayType);

        for (const auto& f : _allFolder)
        {
            RapidJsonValueW folderObj(rapidjson::kObjectType);

            RapidJsonValueW name(f.name.c_str(), f.name.length());
            RapidJsonValueW path(f.path.c_str(), f.path.length());

            folderObj.AddMember(L"Name", name, doc.GetAllocator());
            folderObj.AddMember(L"Path", path, doc.GetAllocator());

            arr.PushBack(folderObj, doc.GetAllocator());
        }

        doc.AddMember(L"FolderArray", arr, doc.GetAllocator());
    }

    RapidJsonStringBufferW sb;
    RapidJsonPrettyWriterW writer {sb};

    doc.Accept(writer);

    fs << doc.GetString();

    fs.close();
}
