#pragma once

#include "imgui.h"
#include "NonCopyable.h"

namespace UiTemplate
{
    class Win32Window;

    class ImGuiLogic : public Util::NonCopyable
    {
    public:
        ImGuiLogic();
        virtual ~ImGuiLogic();

    public:
        void SetUp(Win32Window* pWindow);
        void Loop();
        void Destroy();

    protected:
        virtual void Update() = 0;

    protected:
        // Top Window
        Win32Window* _pTopWindow;
    };
}