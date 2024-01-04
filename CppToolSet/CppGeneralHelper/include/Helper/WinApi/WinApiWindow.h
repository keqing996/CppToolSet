#pragma once

#include <string>

namespace Helper::Win::Window
{
    struct WindowRegisterInfo
    {
        void* hIcon;
        void* hIconSmall;
        void* hCursor;
        void* pWinMsgProc;
    };

    struct WindowCreateStyle
    {
        bool hasCaption;
        bool hasSysmenu;
        bool hasMaxBtn;
        bool hasMinBtn;
    };

    void Register(const std::wstring& windowRegisterName, WindowRegisterInfo info);

    void* Show(const std::wstring& windowRegisterName, const std::wstring& windowTitleName, int width, int height, WindowCreateStyle style);

    void Destroy(void* hWnd);

    void UnRegister(const std::wstring& windowRegisterName);

    void* GetDefaultWinMsgProc();

}