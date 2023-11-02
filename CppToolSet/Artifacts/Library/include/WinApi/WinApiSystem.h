#pragma once

#include "WinApiBaseDefine.h"
#include <string>

namespace WinApi
{
    std::string WinGetUserName();

    bool WinCreateProcess(std::string cmdLine);

}