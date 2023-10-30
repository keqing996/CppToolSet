#pragma once

#include <string>
#include <vector>
#include <functional>
#include <d3d11.h>
#include "Define/WindowsPlatformMinDef.h"
#include "IWinMsgReceiver.h"

namespace UI
{
    class Win32Window
    {
    public:
        Win32Window(const char* windowRegisterName, const char* windowTitle, int width, int height);
        ~Win32Window();

    public:
        bool SetUp();
        void Show();
        void UpdateWinMessage(bool* isQuit);
        void ClearColor();
        void SwapChain();

    public:
        HWND GetWindowHandle() const;

    protected: /* message */
        static LRESULT WINAPI WndProcDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        void AddWinMsgProc(IWinMsgReceiver* pWinMsgReceiver);
        void RemoveWinMsgProc(IWinMsgReceiver* pWinMsgReceiver);

        virtual LRESULT OnWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgSysCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        void Win32RegisterWindow();
        void Win32CreateWindow();
        void Win32DestroyWindow();
        void Win32UnRegisterWindow();
        bool D3dCreateDevice();
        void D3dDestroyDevice();
        void D3dCreateRenderTarget();
        void D3dDestroyRenderTarget();

    private:
        /* Basic */
        int _width;
        int _height;
        HWND _hWnd;
        const char* _windowRegisterName;
        const char* _windowTitle;

        /* Msg Receive */
        std::vector<IWinMsgReceiver*> _winMsgReceiverVec;

        /* D3D */
        ID3D11Device* pD3dDevice = nullptr;
        ID3D11DeviceContext* pD3dDeviceContext = nullptr;
        IDXGISwapChain* pSwapChain = nullptr;
        ID3D11RenderTargetView* pMainRenderTarget = nullptr;
    };
}

