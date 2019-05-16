#pragma once
#include <windows.h>
#include <string>
#include <iostream>


class Log {

public:

	inline static void SetLevel(unsigned int Level);
	inline static void SetColor(unsigned int color);
	inline static void Message(std::string caller,std::string msg);


	static unsigned int LEVEL;

	enum 
	{
		INFO,

		WARN,

		ERR,

		TRACE
	};

	enum
	{
		RED = 20,
		YELLOW = 14,
		WHITE = 15
	};
	
};

#define LOG_INFO(caller,msg) {if(Log::LEVEL == Log::INFO || Log::LEVEL == Log::TRACE) {Log::SetColor(Log::WHITE);Log::Message(caller,msg);}}
#define LOG_WARN(caller,msg) {if(Log::LEVEL == Log::WARN || Log::LEVEL == Log::TRACE) {Log::SetColor(Log::YELLOW);Log::Message(caller, msg);}}
#define LOG_ERR(caller,msg) {if(Log::LEVEL == Log::ERR || Log::LEVEL == Log::TRACE) {Log::SetColor(Log::RED);Log::Message(caller, msg);}}
