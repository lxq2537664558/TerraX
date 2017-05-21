#include "timer.h"

using namespace terra;
void Timer::Tick()
{
    auto time_now = steady_clock::now();
    frame_interval_ = static_cast<int>(duration_cast<milliseconds>(time_now - frame_start_).count());
    frame_start_ = steady_clock::now();
    frame_count_++;
}