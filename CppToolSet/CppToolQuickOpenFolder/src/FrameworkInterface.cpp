
#include "../Framework/Window/WinWindow.h"
#include "MainLogic.h"
#include "../resource/resource.h"

class WindowExample : public UiTemplate::Win32Window
{
protected:
    const char* GetWindowTitle() override
    {
        return "Quick Open Folder - Ver 4.1.0";
    }

    int GetWindowIconResourceId() override
    {
        return IDI_ICON1;
    }

    int GetWindowInitWidth() override
    {
        return 1200;
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
        return true;
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