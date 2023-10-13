
#include <Windows.h>

#include "Application/Application.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev,PWSTR lpCmdLine,int nCmdShow)
{
    Application::CreateInstance();
    Application::GetInstance()->InitWindow(900, 600, L"QuickOpenFolder");
    Application::GetInstance()->RunLoop();
    return 0;
}