

#include "../Window/WinWindow.h"
#include "ImGuiLogic/ImGuiLogic.h"

// Imp Out Of Framework
UiTemplate::Win32Window* CreateMainWindow();
UiTemplate::ImGuiLogic* CreateMainLogic();

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    UiTemplate::Win32Window* pMainWindow = CreateMainWindow();

    if (!pMainWindow->SetUp())
        return 1;

    UiTemplate::ImGuiLogic* pLogic = CreateMainLogic();
    pLogic->SetUp(pMainWindow);

    pMainWindow->Show();

    while (true)
    {
        bool isQuit = false;
        pMainWindow->WinMessageLoop(&isQuit);

        if (isQuit)
            break;

        pMainWindow->RenderLoop();
    }

    pLogic->Destroy();
    pMainWindow->Destroy();

    delete pLogic;
    delete pMainWindow;

    return 0;
}