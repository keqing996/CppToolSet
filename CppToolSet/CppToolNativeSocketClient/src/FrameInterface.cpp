#include "Framework/Window/WinWindow.h"

#include "Window.h"
#include "Logic.h"

UiTemplate::Win32Window* CreateMainWindow()
{
    return new Window();
}

UiTemplate::ImGuiLogic* CreateMainLogic()
{
    return new Logic();
}