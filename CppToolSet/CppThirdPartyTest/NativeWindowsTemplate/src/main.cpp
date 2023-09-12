#include "GlobalDef.h"

#include "Window.hpp"
#include "Application/Application.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev,PWSTR lpCmdLine,int nCmdShow)
{
    try
    {
        Application app(1920, 1080, L"Engine");

        app.Begin();
    }
    catch (const std::exception &e)
    {
        MessageBoxA(nullptr, e.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBoxW(nullptr, L"No Desc Exception", L"Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;

}
