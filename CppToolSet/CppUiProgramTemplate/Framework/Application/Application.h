#pragma once

#include <string>
#include "NonCopyable.h"
#include "../Window/WinWindow.h"
#include "../ImGuiLogic/ImGuiLogic.h"

namespace UiTemplate
{
    class Application: public Util::NonCopyable
    {
    public:
        bool Init();
        void Loop();
        void Destroy();

    public:
        virtual void InitLanguage();
        virtual Win32Window* CreateMainWindow() = 0;
        virtual ImGuiLogic* CreateMainLogic() = 0;
        virtual bool EnableFileLog();
        virtual std::string GetFileLogPath();

    private:
        Win32Window* pMainWindow;
        ImGuiLogic* pLogic;
    };
}