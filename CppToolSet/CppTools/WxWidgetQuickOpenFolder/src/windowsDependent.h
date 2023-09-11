#pragma once

#include <string>

namespace WindowsDependent
{
    std::wstring WinGetUserName();
    void WinShellExecute(std::wstring app, std::wstring appPara, std::wstring appPath);
    void WinShellExecute(std::wstring app, std::wstring appPara);
    bool WinCreateProcess(const std::wstring& cmd);
}
