
#include "Framework/Window/WinWindow.h"
#include "Framework/ImGuiLogic/ImGuiLogic.h"
#include "MainLogic.h"

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
};

UiTemplate::Win32Window* CreateMainWindow()
{
    return new WindowExample();
}

UiTemplate::ImGuiLogic* CreateMainLogic()
{
    return new MainLogic();
}