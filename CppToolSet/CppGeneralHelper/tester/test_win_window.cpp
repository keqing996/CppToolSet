#include <Helper/WinApi/WinApiWindow.h>

int main()
{
    Helper::Win::Window::RegisterInfo info {
        nullptr,
        nullptr,
        nullptr,
        Helper::Win::Window::GetDefaultWinMsgProc()
    };

    Helper::Win::Window::Register(L"Window", info);

    Helper::Win::Window::CreateStyle style {
        true,
        true,
        true,
        true
    };

    void* hWnd = Helper::Win::Window::Show(L"Window", L"Title", 800, 600, style);

    while (true)
    {
        bool quit = Helper::Win::Window::MessageLoop();
        if (quit)
            break;
    }

    Helper::Win::Window::Destroy(hWnd);
    Helper::Win::Window::UnRegister(L"Window");
}