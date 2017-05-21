#pragma once

#include <chrono>
#include "types.h"

using namespace std::chrono;
namespace terra
{
    class Timer
    {
        DISABLE_COPY(Timer);
        MAKE_INSTANCE(Timer);

    private:
        const steady_clock::time_point timer_begin_{steady_clock::now()};
        steady_clock::time_point frame_start_{steady_clock::now()};
        int32_t frame_interval_{0};
        int64_t frame_count_{0};

    public:
        Timer() = default;
        ~Timer() = default;

        void Tick();

        int32_t FrameInterval() const { return frame_interval_; }
        int32_t FrameElapse() const
        {
            return static_cast<int>(duration_cast<milliseconds>(steady_clock::now() - frame_start_).count());
        }

        uint64_t TotalElapse() const
        {
            return duration_cast<milliseconds>(steady_clock::now() - timer_begin_).count();
        }

        steady_clock::time_point Now() { return steady_clock::now(); }
    };
}