
#include "Framework/Window/WinWindow.h"
#include "Framework/ImGuiLogic/ImGuiLogic.h"
#include "MainLogic.h"

class WindowExample : public UiTemplate::Win32Window
{
protected:
    const char* GetWindowTitle() override
    {
        return "Quick Open Folder";
    }

    int GetWindowInitWidth() override
    {
        return 600;
    }

    int GetWindowInitHeight() override
    {
        return 800;
    }

    bool WindowHasMaxBox() override
    {
        return false;
    }

    bool WindowHasMinBox() override
    {
        return false;
    }

    bool WindowCanThickFrame() override
    {
        return false;
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