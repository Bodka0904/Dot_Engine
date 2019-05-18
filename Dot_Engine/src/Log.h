#pragma once
#include <windows.h>
#include <string>
#include <iostream>


class Log {

public:

	inline static void SetLevel(unsigned int Level);
	inline static void SetColor(unsigned int color);
	
	enum 
	{
		INFO,

		WARN,

		ERR,

		TRACE
	};

	static unsigned int LEVEL;
	
	enum
	{
		RED = 20,
		YELLOW = 14,
		WHITE = 15
	};
	
};

#define LOG_INFO(pargs) {if(Log::LEVEL == Log::INFO || Log::LEVEL == Log::TRACE) {Log::SetColor(Log::WHITE); printf pargs;}}
#define LOG_WARN(pargs) {if(Log::LEVEL == Log::WARN || Log::LEVEL == Log::TRACE) {Log::SetColor(Log::YELLOW);printf pargs;}}
#define LOG_ERR(pargs) {if(Log::LEVEL == Log::ERR || Log::LEVEL == Log::TRACE) {Log::SetColor(Log::RED);printf pargs;}}
