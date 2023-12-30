#pragma once

#include <chrono>

namespace Util
{
    template<typename Duration = std::chrono::microseconds>
    class Timer
    {
    public:
        void SetNow()
        {
            _timePoint = std::chrono::steady_clock::now();
        }

        int64_t GetIntervalAndSetNow()
        {
            auto interval = std::chrono::duration_cast<Duration>(
                std::chrono::steady_clock::now() - _timePoint).count();

            SetNow();
            return interval;
        }

        int64_t GetInterval() const
        {
            auto interval = std::chrono::duration_cast<Duration>(
                std::chrono::steady_clock::now() - _timePoint).count();

            return interval;
        }

    private:
        std::chrono::time_point<std::chrono::steady_clock> _timePoint;
    };
}