#ifndef LOG_H
#define LOG_H

#include "Dot/Core/Core.h"
#include "Dot/Core/ColorCodes.h"
#include <string>
#include <fstream>

namespace Dot {
	struct ANSI
	{
		ANSI(int code)
			: m_Code(code)
		{}
		int m_Code;
	};

	
	enum LogLevel
	{
		INFO,
		WARNING,
		ERROR,
		TRACE
	};	


	class Logger
	{
	public:
		static void Init();
		
		void Info(const char* fncname, const char* format, ...);
		void Warn(const char* fncname, const char* format, ...);
		void Error(const char* fncname, const char* format, ...);

		void SetLogLevel(LogLevel level) {m_LogLevel = level; };


		static void SetLogFile(const std::string& logfile);
		static std::shared_ptr<Logger> Get();

		friend std::ostream& operator<<(std::ostream& os, ANSI ansi);
	
	private:
		static int m_LogLevel;	
		static std::ofstream m_LogFile;
		static std::string m_FileName;
		static std::shared_ptr<Logger> m_This;
	};

#define LOG_INFO(...) Logger::Get()->Info(__FUNCTION__,__VA_ARGS__)
#define LOG_WARN(...) Logger::Get()->Warn(__FUNCTION__,__VA_ARGS__)
#define LOG_ERR(...)  Logger::Get()->Error(__FUNCTION__,__VA_ARGS__)
}


#endif