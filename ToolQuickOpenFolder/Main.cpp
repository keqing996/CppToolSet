
#include <locale>
#include "QuickOpenFolder.h"
#include "Resource/Resource.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    std::locale::global(std::locale("zh_CN.UTF8"));

    QuickOpenFolder logic;

    IMWinApp::ImGuiWinApp app(800, 600, "Quick Open Fold - v5.0.1", (int)NWA::WindowStyle::HaveTitleBar | (int)NWA::WindowStyle::HaveClose);

    app.GetNativeWindow().SetIcon(IDI_ICON1);

    logic.LoadChineseFonts(app);

    auto tick = [&](IMWinApp::ImGuiWinApp& window) -> void
    {
        logic.Update();
    };

    app.SetTickFunction<IMWinApp::ImGuiWinApp::TickStage::OnFrame>(tick);
    app.EnableVSync(true);
    app.AppLoop();

    return 0;
}