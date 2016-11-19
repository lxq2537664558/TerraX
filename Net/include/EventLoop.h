#pragma once
#include <event2/event.h>
#include <cassert>

namespace TerraX
{
	class EventLoop // noncopyable
	{
	public:
		EventLoop()
			: m_base(::event_base_new())
		{
			assert(m_base != nullptr);
		}

		~EventLoop()
		{
			::event_base_free(m_base);
		}

		int loop()
		{
			return ::event_base_loop(m_base, EVLOOP_ONCE|EVLOOP_NONBLOCK);
		}

		struct event_base* eventBase()
		{
			return m_base;
		}

	private:
		struct event_base* const m_base{ nullptr };
		// pthread_t 

		EventLoop& operator=(const EventLoop&) = delete;
		EventLoop(const EventLoop&) = delete;
	};
}
