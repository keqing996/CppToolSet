#pragma once

#include <string>

namespace Helper::Win::Window
{
    struct RegisterInfo
    {
        void* hIcon;
        void* hIconSmall;
        void* hCursor;
        void* pWinMsgProc;
    };

    struct SystemMenuStyle
    {
        bool hasSysmenu;
        bool hasMaxBtn;
        bool hasMinBtn;
    };

    void Register(const std::wstring& windowRegisterName, RegisterInfo info);

    void* Show(const std::wstring& windowRegisterName, const std::wstring& windowTitleName, int width, int height, SystemMenuStyle menuStyle);

    void Destroy(void* hWnd);

    void UnRegister(const std::wstring& windowRegisterName);

    bool MessageLoop(bool blockWhenNoWindowsMessage = false);

    void* GetDefaultWinMsgProc();

}