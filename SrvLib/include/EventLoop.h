#pragma once
#include <event2/event.h>
#include <cassert>

namespace TerraX
{
	class EventLoop // noncopyable
	{
	public:
		EventLoop()
			: base_(::event_base_new())
		{
			assert(base_ != NULL);
		}

		~EventLoop()
		{
			::event_base_free(base_);
		}

		int loop()
		{
			return ::event_base_loop(base_, 0);
		}

		struct event_base* eventBase()
		{
			return base_;
		}

	private:
		struct event_base* const base_;
		// pthread_t 

		EventLoop& operator=(const EventLoop&) = delete;
		EventLoop(const EventLoop&) = delete;
	};
}
