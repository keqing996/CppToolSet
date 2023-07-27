

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

#include "../include/utilWin32.h"

namespace Grf::UtilWin32
{
    std::wstring GetUserName()
    {
        wchar_t userNameBuffer[255];
        DWORD userNameBufferLen;
        ::GetUserName(userNameBuffer, &userNameBufferLen);
        return { userNameBuffer };
    }
}