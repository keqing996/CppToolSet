
#include <filesystem>
#include <fstream>

#include "Application/Application.h"
#include "imgui_impl_win32.h"
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

    // Get Dpi Scale
    float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(Application::GetInstance()->GetWindowHandle());
    ImGui::GetStyle().ScaleAllSizes(dpiScale);

    // Load Font
    float fontSize = 20 * dpiScale;

    auto io = ImGui::GetIO();
    _bigFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\consola.ttf", fontSize, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

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
        ImGui::Text(title);
    }
    ImGui::PopFont();

    ImGui::Button("...", {45, 0});
    ImGui::SameLine();

    //static char buf[128] = "click on a button to set focus";

    ImGui::SetNextItemWidth(-1);

    if (_vsCodePathString.empty())
        ImGui::Text((const char*)(u8"中文"));
    else
        ImGui::Text("asdasds");
    //ImGui::InputText("##", _vsCodePathString.c_str(), 0, ImGuiInputTextFlags_ReadOnly);
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
