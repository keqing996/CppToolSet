#pragma once

#include "Define/Types.h"

namespace UI
{
    class IWinMsgReceiver
    {
    public:
        virtual void OnWinMsg(int64 hWnd, uint32 msg, int64 wParam, int64 lParam) = 0;
    };
}