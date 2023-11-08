#pragma once

namespace UiTemplate
{
    class Win32Window;
};

class Window: public UiTemplate::Win32Window
{
protected:
    const char* GetWindowTitle() override;

    int GetWindowIconResourceId() override;

    int GetWindowInitWidth() override;

    int GetWindowInitHeight() override;

    bool WindowHasMaxBox() override;

    bool WindowHasMinBox() override;

    bool WindowCanThickFrame() override;

    bool BlockWhenNoWindowsMessage() override;
};