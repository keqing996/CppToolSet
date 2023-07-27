#pragma once

#include <string>

namespace WindowsDependent
{
    std::wstring WinGetUserName();
    void WinExecute(std::wstring app, std::wstring appPath, std::wstring appPara);
    void WinExecute(std::wstring app, std::wstring appPara);
}
