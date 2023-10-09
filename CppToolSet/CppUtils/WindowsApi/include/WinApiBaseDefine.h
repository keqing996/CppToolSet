#pragma once

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef UNICODE
#define UNICODE
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

namespace WindowsApi
{
    template<typename T>
    struct Coord
    {
        T x;
        T y;
    };

    template<typename T>
    struct Rect
    {
        T left;
        T top;
        T right;
        T bottom;
    };
}