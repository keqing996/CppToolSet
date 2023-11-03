#pragma once

#include <string>
#include <vector>
#include <functional>
#include <d3d11.h>

#include "IWinMsgReceiver.h"
#include "Framework/ImGuiRender/ImGuiRender.h"
#include "Framework/ImGuiLogic/ImGuiLogic.h"
#include "NonCopyable.h"

namespace UiTemplate
{
    class Win32Window: public Util::NonCopyable
    {
    public:
        Win32Window() = default;
        virtual ~Win32Window() = default;

    public:
        virtual bool SetUp();
        virtual void Destroy();
        void Show();
        void WinMessageLoop(bool* isQuit);
        void RenderLoop();
        void AddWinMsgProc(IWinMsgReceiver* pWinMsgReceiver);
        void RemoveWinMsgProc(IWinMsgReceiver* pWinMsgReceiver);
        void AddImGuiLogicUpdater(ImGuiLogic* pUpdater);
        void RemoveImGuiLogicUpdater(ImGuiLogic* pUpdater);

    public: /* getter */
        HWND GetWindowHandle() const;
        ID3D11Device* GetD3dDevice() const;
        ID3D11DeviceContext* GetD3dDeviceContext() const;
        ImGuiRender* GetRender() const;

    protected: /* init para */
        virtual const char* GetWindowRegisterName();
        virtual const char* GetWindowTitle() = 0;
        virtual int GetWindowInitWidth() = 0;
        virtual int GetWindowInitHeight() = 0;

    private: /* message */
        static LRESULT WINAPI WndProcDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        LRESULT OnWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgSysCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private: /* render */
        void ClearColor();
        void SwapChain();

    private:
        void InitLanguage();
        void Win32RegisterWindow();
        void Win32CreateWindow();
        void Win32DestroyWindow();
        void Win32UnRegisterWindow();
        bool D3dCreateDevice();
        void D3dDestroyDevice();
        void D3dCreateRenderTarget();
        void D3dDestroyRenderTarget();
        void ImGuiCreateRender();
        void ImGuiDestroyRender();

    private:
        /* Basic */
        int _width;
        int _height;
        HWND _hWnd;
        std::wstring _windowRegisterName;
        std::wstring _windowTitle;

        /* Msg Receive */
        std::vector<IWinMsgReceiver*> _winMsgReceiverVec;

        /* D3D */
        ID3D11Device* _pD3dDevice = nullptr;
        ID3D11DeviceContext* _pD3dDeviceContext = nullptr;
        IDXGISwapChain* _pSwapChain = nullptr;
        ID3D11RenderTargetView* _pMainRenderTarget = nullptr;

        /* ImGuiRender */
        ImGuiRender* _pImGuiRender = nullptr;

        /* ImGuiLogic */
        std::vector<ImGuiLogic*> _imGuiLogicVec;
    };
}

