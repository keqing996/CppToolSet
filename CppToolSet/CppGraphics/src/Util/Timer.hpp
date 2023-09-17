#pragma once

#include <chrono>

namespace Util
{
    class Timer
    {
    public:
        void SetNow()
        {
            _timePoint = std::chrono::steady_clock::now();
        }

        int64_t GetIntervalMsAndSetNow()
        {
            auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _timePoint).count();
            SetNow();
            return interval;
        }

        int64_t GetInterval()
        {
            auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _timePoint).count();
            return interval;
        }

    private:
        std::chrono::time_point<std::chrono::steady_clock> _timePoint;
    };
}