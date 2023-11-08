
#include "Framework/Window/WinWindow.h"
#include "../resource/resource.h"
#include "Window.h"

const char* Window::GetWindowTitle()
{
    return "Socket Client - Ver 1.0.0";
}

int Window::GetWindowIconResourceId()
{
    return IDI_ICON1;
}

int Window::GetWindowInitWidth()
{
    return 600;
}

int Window::GetWindowInitHeight()
{
    return 800;
}

bool Window::WindowHasMaxBox()
{
    return false;
}

bool Window::WindowHasMinBox()
{
    return true;
}

bool Window::WindowCanThickFrame()
{
    return false;
}

bool Window::BlockWhenNoWindowsMessage()
{
    return true;
}
