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
	enum
	{
		INFO,

		WARN,

		ERR,

		TRACE
	};

private:

	static int LEVEL;
};




#define LOG_INFO(...) if(Log::GetLevel() == Log::INFO || Log::TRACE) printf("%c[1;0m",27);printf(__VA_ARGS__);printf("\n");
#define LOG_WARN(...) if(Log::GetLevel() == Log::WARN || Log::TRACE) printf("%c[1;33m",27);printf(__VA_ARGS__);printf("\n");
#define LOG_ERR(...)  if(Log::GetLevel() == Log::ERR || Log::TRACE) printf("%c[1;31m",27);printf(__VA_ARGS__);printf("\n");

#endif