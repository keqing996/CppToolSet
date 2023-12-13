
#include <memory>
#include <array>
#include <string>
#include <vector>
#include "../Framework/Application/Application.h"
#include "../resource/resource.h"
#include "WinApi/WinApiFileDialog.h"
#include "imgui_internal.h"

class WindowExample : public UiTemplate::Win32Window
{
protected:
    const char* GetWindowTitle() override
    {
        return "UiExample";
    }

    int GetWindowInitWidth() override
    {
        return 1200;
    }

    int GetWindowInitHeight() override
    {
        return 800;
    }

    int GetWindowIconResourceId() override
    {
        return IDI_ICON1;
    }
};

class LogicExample : public UiTemplate::ImGuiLogic
{
protected:
    inline static float f = 0.0f;
    inline static int counter = 0;
    inline static bool show_demo_window = false;
    inline static bool show_another_window = false;
    inline static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    inline static char input_array[256];

    void Update() override
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        int sidebarMinWidth = 250;
        int sidebarWidth = main_viewport->WorkSize.x * 0.2 < sidebarMinWidth ? sidebarMinWidth : main_viewport->WorkSize.x * 0.2;
        ImVec2 sidebarSize = ImVec2(sidebarWidth, main_viewport->WorkSize.y);
        ImVec2 sidebarPos = ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y);

        ImVec2 mainWindowSize = ImVec2(main_viewport->WorkSize.x - sidebarWidth, main_viewport->WorkSize.y);
        ImVec2 mainWindowPos = ImVec2(main_viewport->WorkPos.x + sidebarWidth, main_viewport->WorkPos.y);

        // Sidebar

        ImGui::SetNextWindowSize(sidebarSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(sidebarPos, ImGuiCond_Always);

        ImGui::Begin("Sidebar", nullptr, window_flags);

        UpdateSidebar();

        ImGui::End();

        // MainWindow

        ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(mainWindowPos, ImGuiCond_Always);

        ImGui::Begin("MainWindow", nullptr, window_flags);

        UpdateMainWindow();

        ImGui::End();

    }

    void UpdateSidebar()
    {
        ImGui::Dummy(ImVec2 {0, 15});

        // disable tab button
        static int current_tab = 0;
        DisableButton("TestButton0", [&](){return current_tab == 0;}, [&](){current_tab = 0;});
        DisableButton("TestButton1", [&](){return current_tab == 1;}, [&](){current_tab = 1;});
        DisableButton("TestButton2", [&](){return current_tab == 2;}, [&](){current_tab = 2;});

        ImGui::Dummy(ImVec2 {0, 15});

        // center label & input
        float available = ImGui::GetContentRegionAvail().x;
        float size = ImGui::CalcTextSize("Center").x + ImGui::GetStyle().FramePadding.x;
        float cursor_offset = 0.5f * (available - size);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + cursor_offset);
        ImGui::Text("Center");

        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##Input", input_array, 256,
                         ImGuiInputTextFlags_NoHorizontalScroll
                         | ImGuiInputTextFlags_EscapeClearsAll
                         | ImGuiInputTextFlags_AutoSelectAll);

        ImGui::Dummy(ImVec2 {0, 15});

    }

    void UpdateMainWindow()
    {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::PushFont(_pTopWindow->GetRender()->GetBoldFontLarge());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::PopFont();

        UpdateMessageSubWindow();
        UpdateCommandLine();
    }

    void UpdateMessageSubWindow()
    {
        ImVec2 avail_space = ImGui::GetContentRegionAvail();
        float commandline_height = ImGui::CalcTextSize("I").y + ImGui::GetStyle().FramePadding.y * 4.f;

        ImGui::BeginChild("terminal:logs_window",
                          ImVec2(avail_space.x, avail_space.y - commandline_height - 2),
                          true,
                          ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar);

        ImGui::TextWrapped("Equivalent to multiple alignas specifiers applied to the same declaration, one for each member of the parameter pack, which can be either type or non-type parameter pack.");
        ImGui::TextWrapped("The object or the type declared by such a declaration will have its alignment requirement equal to the strictest (largest) non-zero expression of all alignas specifiers used in the declaration, unless it would weaken the natural alignment of the type.");
        ImGui::TextWrapped("If the strictest (largest) alignas on a declaration is weaker than the alignment it would have without any alignas specifiers (that is, weaker than its natural alignment or weaker than alignas on another declaration of the same object or type), the program is ill-formed");
        ImGui::TextWrapped("Invalid non-zero alignments, such as alignas(3) are ill-formed.");
        ImGui::TextWrapped("As of the ISO C11 standard, the C language has the _Alignas keyword and defines alignas as a preprocessor macro expanding to the keyword in the header <stdalign.h>.");
        //ImGui::SetScrollHereY(1.f);

        ImGui::EndChild();
    }

    void UpdateCommandLine()
    {
        static std::array<char, 1024> commandLineBuffer {};

        ImGui::Separator();

        ImGui::SetNextItemWidth(-1.f);
        ImGui::InputText("##terminal:input_text", commandLineBuffer.data(), commandLineBuffer.size());
    }

    void DisableButton(const char* label, std::function<bool()> disable, std::function<void()> callback)
    {
        bool should_disable = disable();
        if (should_disable)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        if (ImGui::Button(label, {-1, 0}))
            callback();

        if (should_disable)
        {
            ImGui::PopStyleVar();
            ImGui::PopItemFlag();
        }
    }
};

class ApplicationExample: public UiTemplate::Application
{
public:
    UiTemplate::Win32Window* CreateMainWindow() override
    {
        return new WindowExample();
    }

    UiTemplate::ImGuiLogic* CreateMainLogic() override
    {
        return new LogicExample();
    }

    bool EnableFileLog() override
    {
        return true;
    }

    std::string GetFileLogPath() override
    {
        return "log.txt";
    }
};

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    std::shared_ptr<UiTemplate::Application> pApp = std::make_shared<ApplicationExample>();

    if (!pApp->Init())
        return 1;

    pApp->Loop();

    pApp->Destroy();

    return 0;
}