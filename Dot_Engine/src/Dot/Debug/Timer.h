#pragma once
#include "Log.h"

struct Members;

class Timer
{
public:
	Timer();
	~Timer();

	void Reset();

	float Elapsed();
	void FrameCount();


private:
	Members* m_Members;

};