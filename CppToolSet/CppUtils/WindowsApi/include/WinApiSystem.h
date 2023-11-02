#pragma once

#include <string>

namespace WinApi::System
{
    std::wstring GetWindowsUserName();

    bool CreateDependentProcess(const std::wstring& cmdLine);

}