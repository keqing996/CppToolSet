#include "Application.h"
#include "Logger.h"

namespace UiTemplate
{

    bool Application::Init()
    {
        // Language
        InitLanguage();

        // Logger
        Util::Logger::InitConsoleLogger();

        if (EnableFileLog())
        {
            Util::Logger::InitFileLogger(GetFileLogPath());
        }

        // Main Window
        Win32Window* pMainWindow = CreateMainWindow();

        if (!pMainWindow->SetUp())
            return false;

        // Logic
        ImGuiLogic* pLogic = CreateMainLogic();
        pLogic->SetUp(pMainWindow);

        pMainWindow->Show();

        return true;
    }

    void Application::Loop()
    {
        while (true)
        {
            bool isQuit = false;
            pMainWindow->WinMessageLoop(&isQuit);

            if (isQuit)
                break;

            pMainWindow->RenderLoop();
        }
    }

    void Application::Destroy()
    {
        pLogic->Destroy();
        pMainWindow->Destroy();

        delete pLogic;
        delete pMainWindow;

        Util::Logger::Close();
    }

    void Application::InitLanguage()
    {
        std::locale::global(std::locale("zh_CN.UTF8"));
    }

    bool Application::EnableFileLog()
    {
        return false;
    }

    std::string Application::GetFileLogPath()
    {
        return "log.txt";
    }
}