
#include <locale>
#include <Infra/Windows/WindowsDefine.hpp>
#include "TargtInputWindow.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    TargetInputWindow* pInputWindow = new TargetInputWindow();

    pInputWindow->Loop();

    return 0;
}