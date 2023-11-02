#pragma once

#include <d3d11.h>
#include "WinApi/WindowsPlatform.h"
#include "Window/IWinMsgReceiver.h"
#include "NonCopyable.h"

struct ImFontAtlas;
struct ImFont;

namespace UI
{
class ImGuiRender : public IWinMsgReceiver, public Util::NonCopyable
    {
    public:
        ImGuiRender();
        ~ImGuiRender();
        void OnWinMsg(int64 hWnd, uint32 msg, int64 wParam, int64 lParam) override;

    public:
        void SetUp(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
        void Destroy();
        void NewFrame();
        void EndFrame();
        void RenderDrawData();

    public:
        float GetDpiScale(int64 hWnd) const;
        ImFont* GetNormalFont() const;
        ImFont* GetLargeFont() const;

    private:
        static constexpr int NORMAL_FONT_SIZE = 16;
        static constexpr int LARGE_FONT_SIZE = 20;
        static inline ImFontAtlas* _pSharedImGuiFonts = nullptr;
        static void InitSharedFontAtlas();

    private:
        ImFont* _pFontNormalJetbrainsMonoMsYaHei = nullptr;
        ImFont* _pFontLargeJetbrainsMonoMsYaHei = nullptr;

    };
}