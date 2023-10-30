#pragma once

#include "Define/WindowsPlatformMinDef.h"

namespace UI
{
    class IWinMsgReceiver
    {
    public:
        virtual void OnWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
    };
}