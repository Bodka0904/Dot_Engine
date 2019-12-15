#pragma once
#include "Dot/Debug/Log.h"
#include <chrono>

namespace Dot {
	class Timer
	{
	public:
		Timer();
		~Timer();

		void Stop();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	};
}

