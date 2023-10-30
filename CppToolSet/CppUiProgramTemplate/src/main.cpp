

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

#include "Window/WinWindow.h"
#include "ImGuiRender/ImGuiRender.h"
#include "ImGuiLogic/ImGuiLogic.h"

// Main code
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev,PWSTR lpCmdLine,int nCmdShow)
{
    UI::Win32Window mainWin("MainWindow", "UiWindow", 1200, 800);

    if (!mainWin.SetUp())
        return 1;

    // Show the window
    mainWin.Show();

    UI::ImGuiLogic imGuiLogic;
    imGuiLogic.SetUp();

    mainWin.AddWinMsgProc(&imGuiLogic);

    UI::ImGuiRender imGuiRender;
    imGuiRender.SetUp(mainWin.GetWindowHandle(), mainWin.GetD3dDevice(), mainWin.GetD3dDeviceContext());

    while (true)
    {
        bool isQuit = false;
        mainWin.UpdateWinMessage(&isQuit);

        if (isQuit)
            break;

        // Start the Dear ImGui frame
        imGuiRender.NewFrame();
        imGuiLogic.NewFrame();

        imGuiLogic.Update();

        imGuiLogic.EndFrame();

        mainWin.ClearColor();
        imGuiRender.RenderDrawData();
        mainWin.SwapChain();
    }

    mainWin.RemoveWinMsgProc(&imGuiLogic);

    return 0;
}