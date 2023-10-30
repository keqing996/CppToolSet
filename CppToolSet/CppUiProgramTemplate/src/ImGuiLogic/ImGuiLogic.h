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

        void OnWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
    };
}