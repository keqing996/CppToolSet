#pragma once

#include "Window/IWinMsgReceiver.h"

namespace UI
{
    class ImGuiLogic : public IWinMsgReceiver
    {
    public:
        ImGuiLogic();
        ~ImGuiLogic();

    public:
        void SetUp();
        void NewFrame();
        virtual void Update();
        void EndFrame();

        void OnWinMsg(int64 hWnd, uint32 msg, int64 wParam, int64 lParam) override;
    };
}