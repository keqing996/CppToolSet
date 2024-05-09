
#include <locale>
#include "QuickOpenFolder.h"

int main()
{
    std::locale::global(std::locale("zh_CN.UTF8"));

    QuickOpenFolder logic;

    Infra::ImGuiWinApp app(800, 600, "Quick Open Fold - v5.0.0", (int)Infra::WindowStyle::HaveTitleBar | (int)Infra::WindowStyle::HaveClose);

    auto tick = [&](Infra::ImGuiWinApp& window) -> void
    {
        logic.Update(window);
    };

    app.SetTickFunction<Infra::ImGuiWinApp::TickStage::OnFrame>(tick);
    app.EnableVSync(true);
    app.AppLoop();

    return 0;
}