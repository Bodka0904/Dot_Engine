#include "stdafx.h"
#include "Dot/Utils/Timer.h"

#ifdef D_PLATFORM_WINDOWS
#include <Windows.h>

struct Members
{
	LARGE_INTEGER m_start;
	double m_frequency;
	float m_timer;
	unsigned int m_frames;
	
};

Timer::Timer()
{
	m_Members = new Members();
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	m_Members->m_frequency = 1.0 / frequency.QuadPart;
	QueryPerformanceCounter(&m_Members->m_start);
}


Timer::~Timer()
{
}

void Timer::Reset()
{
	QueryPerformanceCounter(&m_Members->m_start);
}

float Timer::Elapsed()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	unsigned _int64 cycles = current.QuadPart - m_Members->m_start.QuadPart;

	return (float)(cycles* m_Members->m_frequency);
}

void Timer::FrameCount()
{
	m_Members->m_frames++;
	if (Elapsed() - m_Members->m_timer > 1.0f)
	{
		m_Members->m_timer += 1.0f;
		LOG_INFO("FPS: %d", m_Members->m_frames);
		m_Members->m_frames = 0;
	}
}
#endif