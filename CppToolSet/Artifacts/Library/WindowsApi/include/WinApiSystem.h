#pragma once

#include "WinApiBaseDefine.h"
#include <string>

namespace WindowsApi
{
    std::wstring WinGetUserName();

    bool WinCreateProcess(std::wstring cmdLine);

}