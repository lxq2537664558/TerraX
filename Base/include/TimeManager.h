#pragma once
#include <chrono>
#include <cstdint>
#include "ComDef.h"

using namespace std::chrono;
namespace TerraX
{
	class TimeManager
	{
		NOCOPY(TimeManager);
		MAKEINSTANCE(TimeManager);

	public:
		TimeManager() = default;
		~TimeManager() = default;

		void Start()
		{
			auto time_now = steady_clock::now();
			m_FrameTime = static_cast<int>(duration_cast<milliseconds>(time_now - m_FrameStartTime).count());
			m_FrameStartTime = steady_clock::now();
		}

		int32_t FrameInterval() const { return m_FrameTime; }
		int32_t FrameTimePoint() const
		{
			return static_cast<int>(
				duration_cast<milliseconds>(steady_clock::now() - m_FrameStartTime).count());
		}

		int32_t TotalTime() const
		{
			return static_cast<int>(duration_cast<milliseconds>(steady_clock::now() - m_StartTime).count());
		}

		steady_clock::time_point Now() { return steady_clock::now(); }

	private:
		const steady_clock::time_point m_StartTime{ steady_clock::now() };
		steady_clock::time_point m_FrameStartTime{ steady_clock::now() };
		int32_t m_FrameTime{ 0 };
	};
}