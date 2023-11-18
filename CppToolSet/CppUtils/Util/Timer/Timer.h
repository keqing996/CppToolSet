#pragma once

#include <chrono>

namespace Util
{
    class Timer
    {
    public:
        void SetNow();

        int64_t GetIntervalMsAndSetNow();

        int64_t GetInterval();

    private:
        std::chrono::time_point<std::chrono::steady_clock> _timePoint;
    };
}