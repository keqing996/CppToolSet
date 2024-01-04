#include "Application.h"
#include <Helper/Logger.h>

namespace UiTemplate
{

    bool Application::Init()
    {
        // Language
        InitLanguage();

        // Logger
        Helper::Logger::InitConsoleLogger();

        if (EnableFileLog())
        {
            Helper::Logger::InitFileLogger(GetFileLogPath());
        }

        // Main Window
        pMainWindow = CreateMainWindow();

        if (!pMainWindow->SetUp())
            return false;

        // Logic
        pLogic = CreateMainLogic();
        pLogic->SetUp(pMainWindow);

        pMainWindow->Show();

        Helper::Logger::LogInfo("[UiTemplate] Main window & logic init finish.");

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

        Helper::Logger::Close();
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