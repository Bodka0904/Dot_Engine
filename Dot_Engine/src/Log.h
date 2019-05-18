#include <windows.h>
#include <string>
#include <iostream>

class LogCore {

public:
	inline static void SetLevel(unsigned int Level) {
		if (Level > TRACE)
		{
			LEVEL = TRACE;
		}
		else
		{
			LEVEL = Level;
		}

	}
	static inline void SetColor(unsigned int color) {

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	template <typename dataType>
	static void PrintINFO(const char *message, dataType arg = 0) {
		if (LEVEL == INFO || TRACE)
		{
			SetColor(WHITE);
			printf(message, arg);
		}
	}
	template <typename dataType>
	static void PrintWARN(const char *message, dataType arg = 0) {
		if (LEVEL == WARN || TRACE)
		{
			SetColor(YELLOW);
			printf(message, arg);
		}
	}
	template <typename dataType>
	static void PrintERR(const char *message, dataType arg = 0) {
		if (LEVEL == ERR || TRACE)
		{
			SetColor(RED);
			printf(message, arg);
		}
	}

	enum
	{
		INFO,

		WARN,

		ERR,

		TRACE
	};

private:
	enum
	{
		RED = 20,
		YELLOW = 14,
		WHITE = 15
	};

	static int LEVEL;
};

class Log {

public:
	static inline std::shared_ptr<LogCore> &GetLogCore() { return log; }

private:
	static std::shared_ptr<LogCore> log;
};



#define LOG_INFO(...) Log::GetLogCore()->PrintINFO(__VA_ARGS__);
#define LOG_WARN(...) Log::GetLogCore()->PrintWARN(__VA_ARGS__);
#define LOG_ERR(...)  Log::GetLogCore()->PrintWARN(__VA_ARGS__);