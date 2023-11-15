
#include <memory>
#include "Framework/Application/Application.h"
#include "MainLogic.h"
#include "../resource/resource.h"

class WindowExample : public UiTemplate::Win32Window
{
protected:
    const char* GetWindowTitle() override
    {
        return "Quick Open Folder - Ver 4.2.0";
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

    bool BlockWhenNoWindowsMessage() override
    {
        return true;
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
        return new MainLogic();
    }

    bool EnableFileLog() override
    {
        return false;
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