#pragma once

#include "imgui.h"
#include "NonCopyable.h"

namespace UiTemplate
{
    class Win32Window;

    class ImGuiLogic : public Util::NonCopyable
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