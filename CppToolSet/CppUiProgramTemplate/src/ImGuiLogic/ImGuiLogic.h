#pragma once

#include "imgui.h"

namespace UI
{
    class Win32Window;

    class ImGuiLogic
    {
    public:
        explicit ImGuiLogic(Win32Window* topWindow);
        virtual ~ImGuiLogic();

    public:
        virtual void Update();

    protected:
        // Top Window
        Win32Window* _pTopWindow;
    };
}