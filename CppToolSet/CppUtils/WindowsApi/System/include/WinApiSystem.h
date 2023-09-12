#pragma once

#include <string>

namespace WindowsApi
{
    std::wstring WinGetUserName();

    bool WinCreateProcess(std::wstring cmdLine);

}