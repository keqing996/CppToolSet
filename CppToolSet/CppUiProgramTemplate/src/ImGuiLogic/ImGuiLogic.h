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

        // Font
        static constexpr int NORMAL_FONT_SIZE = 16;
        static constexpr int LARGE_FONT_SIZE = 20;
        ImFont* _pFontNormalMsYaHei = nullptr;
        ImFont* _pFontLargeMsYaHei = nullptr;

    };
}