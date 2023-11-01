#include "Window/WinWindow.h"

// Main code
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev,PWSTR lpCmdLine,int nCmdShow)
{
    UI::Win32Window mainWin("MainWindow", "UiWindow", 1200, 800);

    if (!mainWin.SetUp())
        return 1;

    mainWin.Show();

    while (true)
    {
        bool isQuit = false;
        mainWin.WinMessageLoop(&isQuit);

        if (isQuit)
            break;

        mainWin.RenderLoop();
    }

    return 0;
}