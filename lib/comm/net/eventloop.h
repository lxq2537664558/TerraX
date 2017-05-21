#pragma once

#include <event2/event.h>
#include "base/types.h"

namespace terra
{
    class EventLoop
    {
        DISABLE_COPY(EventLoop);

    private:
        struct event_base* const base_{nullptr};

    public:
        EventLoop() : base_(::event_base_new()) { assert(base_ != nullptr); }

        ~EventLoop() { ::event_base_free(base_); }

        int loop() { return ::event_base_loop(base_, EVLOOP_ONCE | EVLOOP_NONBLOCK); }

        struct event_base* eventBase() { return base_; }
    };
}
