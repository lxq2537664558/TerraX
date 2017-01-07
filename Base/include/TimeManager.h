#pragma once
#include <chrono>
#include <cstdint>
#include "ComDef.h"

using namespace std::chrono;
namespace TerraX
{
	class TimeManager
	{
		DISABLE_COPY(TimeManager);
		MAKE_INSTANCE(TimeManager);

	public:
		TimeManager() = default;
		~TimeManager() = default;

		void Tick()
		{
			auto time_now = steady_clock::now();
			m_FrameTime = static_cast<int>(duration_cast<milliseconds>(time_now - m_FrameStartTime).count());
			m_FrameStartTime = steady_clock::now();
			m_FrameCount++;
		}

		int32_t FrameInterval() const { return m_FrameTime; }
		int32_t FrameTimePoint() const
		{
			return static_cast<int>(
				duration_cast<milliseconds>(steady_clock::now() - m_FrameStartTime).count());
		}

		uint64_t TotalRunTime() const
		{
			return duration_cast<milliseconds>(steady_clock::now() - m_BeginTime).count();
		}

		steady_clock::time_point Now() { return steady_clock::now(); }

	private:
		const steady_clock::time_point m_BeginTime{ steady_clock::now() };
		steady_clock::time_point m_FrameStartTime{ steady_clock::now() };
		int32_t m_FrameTime{ 0 };
		int64_t m_FrameCount{ 0 };
	};
}