#pragma once

#include <string>
#include <vector>
#include <functional>
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
        void Show();
        void Update(bool* isQuit);

    public:
        HWND GetWindowHandle() const;

    public: /* message */
        static LRESULT WINAPI WndProcDispatch(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        void AddWinMsgProc(IWinMsgReceiver* pWinMsgReceiver);
        void RemoveWinMsgProc(IWinMsgReceiver* pWinMsgReceiver);

        virtual LRESULT OnWinMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgSize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgSysCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMsgDestroy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        int _width;
        int _height;

        HWND _hWnd;
        const char* _windowRegisterName;

        std::vector<IWinMsgReceiver*> _winMsgReceiverVec;
    };
}

