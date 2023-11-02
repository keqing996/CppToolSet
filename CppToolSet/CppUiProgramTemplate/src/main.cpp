#include "Window/WinWindow.h"
#include "ImGuiLogic/ImGuiLogic.h"
#include "Util/DebugConsole.h"

// Main code
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev,PWSTR lpCmdLine,int nCmdShow)
{
    std::locale::global(std::locale("zh_CN.UTF8"));
    Util::DebugConsole::Init();

    UI::Win32Window mainWin("MainWindow", "UiWindow", 1200, 800);

    if (!mainWin.SetUp())
        return 1;

    Util::DebugConsole::LogInfo("Main Window Set Up: {}", true);

    UI::ImGuiLogic* pLogic = new UI::ImGuiLogic(&mainWin);

    mainWin.Show();

    while (true)
    {
        bool isQuit = false;
        mainWin.WinMessageLoop(&isQuit);

        if (isQuit)
            break;

        mainWin.RenderLoop();
    }

    delete pLogic;

    return 0;
}