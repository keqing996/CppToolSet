#pragma once

#include <chrono>

namespace Helper
{
    class Timer
    {
    public:
        void SetNow();

        int64_t GetIntervalAndSetNow();

        int64_t GetInterval();

    private:
        std::chrono::time_point<std::chrono::steady_clock> _timePoint;
    };
}