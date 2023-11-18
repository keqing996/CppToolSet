#pragma once

#include <queue>

namespace Util
{
    template<typename T>
    static void TrimQueue(std::queue<T> q, int size)
    {
        while (q.size() > size)
            q.pop();
    }
}

