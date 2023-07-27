#include "windowsDependent.h"

#define NOMINMAX
#include <Windows.h>

namespace WindowsDependent
{
    std::wstring WinGetUserName()
    {
        wchar_t userNameBuffer[255];
        DWORD userNameBufferLen;
        ::GetUserName(userNameBuffer, &userNameBufferLen);
        return {userNameBuffer};
    }

    void WinExecute(std::wstring app, std::wstring appPath, std::wstring appPara)
    {
        ::ShellExecute(
            nullptr,
            nullptr,
            app.c_str(),
            appPara.c_str(),
            appPath.c_str(),
            SW_SHOWNORMAL
            );
    }

    void WinExecute(std::wstring app, std::wstring appPara)
    {
        ::ShellExecute(
            nullptr,
            nullptr,
            app.c_str(),
            appPara.c_str(),
            nullptr,
            SW_SHOWNORMAL
            );
    }
}
