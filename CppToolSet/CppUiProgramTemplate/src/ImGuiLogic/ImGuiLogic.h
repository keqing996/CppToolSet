#pragma once

#include "imgui.h"
#include "Util/NonCopyable.h"

namespace UI
{
    class Win32Window;

    class ImGuiLogic : public NonCopyable
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