
#include <locale>
#include <Infra/Windows/WindowsDefine.hpp>
#include "SocketClient.hpp"
#include "Resource/Resource.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR lpCmdLine, int nCmdShow)
{
    std::locale::global(std::locale("zh_CN.UTF8"));

    Infra::ImGuiWinApp app(800, 600, "Socket Clint", (int)Infra::WindowStyle::HaveTitleBar | (int)Infra::WindowStyle::HaveClose);

    app.GetNativeWindow().SetIcon(IDI_ICON1);

    //app.SetTickFunction<Infra::ImGuiWinApp::TickStage::OnFrame>(tick);
    app.EnableVSync(true);
    app.AppLoop();

    return 0;
}