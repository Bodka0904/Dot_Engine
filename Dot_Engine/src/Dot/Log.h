#include <windows.h>
#include <string>
#include <iostream>

#ifndef LOG_H
#define LOG_H
class Log {

public:
	inline static int &GetLevel() {
		if (LEVEL > TRACE)
		{
			LEVEL = TRACE;
			return LEVEL;
		}

		return LEVEL;

	}
	static inline void SetColor(unsigned int color) {

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	enum
	{
		INFO,

		WARN,

		ERR,

		TRACE
	};
	enum
	{
		RED = 12,
		YELLOW = 14,
		WHITE = 15
	};

private:

	static int LEVEL;
};




#define LOG_INFO(...) Log::SetColor(Log::WHITE);if(Log::GetLevel() == Log::INFO || Log::TRACE)printf(__VA_ARGS__);printf("\n");
#define LOG_WARN(...) Log::SetColor(Log::YELLOW);if(Log::GetLevel() == Log::WARN || Log::TRACE)printf(__VA_ARGS__);printf("\n");
#define LOG_ERR(...)  Log::SetColor(Log::RED);if(Log::GetLevel() == Log::ERR || Log::TRACE)printf(__VA_ARGS__);printf("\n");

#endif