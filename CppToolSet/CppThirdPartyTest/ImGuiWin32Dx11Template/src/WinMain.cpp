
#include <Windows.h>

#include "Application/Application.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev,PWSTR lpCmdLine,int nCmdShow)
{
    Application::CreateInstance();

    if (!Application::GetInstance()->InitWindow(900, 600, L"Renderer"))
        return 1;

    Application::GetInstance()->RunLoop();

    Application::GetInstance()->Destroy();

    return 0;
}