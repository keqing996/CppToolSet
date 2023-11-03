
#include <locale>

#include "Framework/Window/WinWindow.h"
#include "Framework/ImGuiLogic/ImGuiLogic.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    std::locale::global(std::locale("zh_CN.UTF8"));

    UiTemplate::Win32Window mainWin("MainWindow", "UiWindow", 1200, 800);

    if (!mainWin.SetUp())
        return 1;

    UiTemplate::ImGuiLogic* pLogic = new UiTemplate::ImGuiLogic(&mainWin);

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