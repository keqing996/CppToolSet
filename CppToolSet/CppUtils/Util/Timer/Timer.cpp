#include "Timer.h"

namespace Util
{

    void Timer::SetNow()
    {
        _timePoint = std::chrono::steady_clock::now();
    }

    int64_t Timer::GetIntervalAndSetNow()
    {
        auto interval = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _timePoint).count();
        SetNow();
        return interval;
    }

    int64_t Timer::GetInterval()
    {
        auto interval = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _timePoint).count();
        return interval;
    }

}