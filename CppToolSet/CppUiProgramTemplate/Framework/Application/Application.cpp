#include "Application.h"
#include "Logger/Logger.h"

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
        pMainWindow = CreateMainWindow();

        if (!pMainWindow->SetUp())
            return false;

        // Logic
        pLogic = CreateMainLogic();
        pLogic->SetUp(pMainWindow);

        pMainWindow->Show();

        Util::Logger::LogInfo("[UiTemplate] Main window & logic init finish.");

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