
#include <locale>
#include <Infra/Socket.hpp>
#include <Infra/ScopeGuard.hpp>
#include <Infra/Windows/WindowsDefine.hpp>
#include "TargtInputWindow.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    bool socketInitSuccess = Infra::Socket::InitEnvironment();
    if (!socketInitSuccess)
        return 1;

    Infra::ScopeGuard guard([&]() -> void
    {
        Infra::Socket::DestroyEnvironment();
    });

    TargetInputWindow* pInputWindow = new TargetInputWindow();

    pInputWindow->Loop();



    return 0;
}