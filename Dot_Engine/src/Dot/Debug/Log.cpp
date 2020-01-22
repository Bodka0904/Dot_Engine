#include "stdafx.h"
#include "Log.h"
#include "Dot/Utils/Time/TimeUtils.h"
#include <cstdarg>

#include "Dot/Gui/Gui/Widgets/Console.h"

namespace Dot {
	int						Logger::m_LogLevel = TRACE;
	std::string				Logger::m_FileName = "Log.txt";
	std::ofstream			Logger::m_LogFile;
	std::shared_ptr<Logger>	Logger::m_This;


	void Logger::Init()
	{
		m_This = std::make_shared<Logger>();
		m_LogFile.open(m_FileName.c_str(), std::ios::out | std::ios::app);
	}

	void Logger::Info(const char* fncname, const char* format, ...)
	{
		if (m_LogLevel >= LogLevel::INFO)
		{
			char* log = NULL;
			int length = 0;

			va_list args;
			va_start(args, format);

			//  Return the number of characters in the string referenced the list of arguments.
			// _vscprintf doesn't count terminating '\0' (that's why +1)
			length = _vscprintf(format, args) + 1;
			log = new char[length];
			vsprintf_s(log, length, format, args);

			std::string date = Dot::CurrentDateTime();
			m_LogFile << date << ": " << log << std::endl;
			if (m_Console.get())
				m_Console->PushText(date + ": " + log + "\n",glm::vec3(0.2,1,0.4));
			
			std::cout << ANSI(ANSI_COLOR_GREEN_F)<< "INFO: " << date << ": " << fncname << ": " << log << ANSI(ANSI_COLOR_WHITE_F) << std::endl;

			m_LogFile.flush();
			va_end(args);
			delete[] log;
		}
	}

	void Logger::Warn(const char* fncname, const char* format, ...)
	{
		if (m_LogLevel >= LogLevel::WARNING)
		{
			char* log = NULL;
			int length = 0;

			va_list args;
			va_start(args, format);

			//  Return the number of characters in the string referenced the list of arguments.
			// _vscprintf doesn't count terminating '\0' (that's why +1)
			length = _vscprintf(format, args) + 1;
			log = new char[length];
			vsprintf_s(log, length, format, args);

			std::string date = Dot::CurrentDateTime();
			m_LogFile << date << ": " << log << std::endl;
			if (m_Console.get())
				m_Console->PushText(date + ": " + log + "\n",glm::vec3(0.6,0.7,0));
			std::cout << ANSI(ANSI_COLOR_YELLOW_F) << "WARN: " << date << ": " << fncname << ": " << log << ANSI(ANSI_COLOR_WHITE_F) << std::endl;

			m_LogFile.flush();
			va_end(args);
			delete[] log;
		}
	}

	void Logger::Error(const char* fncname, const char* format, ...)
	{
		if (m_LogLevel >= LogLevel::ERROR)
		{
			char* log = NULL;
			int length = 0;

			va_list args;
			va_start(args, format);

			//  Return the number of characters in the string referenced the list of arguments.
			// _vscprintf doesn't count terminating '\0' (that's why +1)
			length = _vscprintf(format, args) + 1;
			log = new char[length];
			vsprintf_s(log, length, format, args);

			std::string date = Dot::CurrentDateTime();
			m_LogFile << date << ": " << log << std::endl;
			if (m_Console.get())
				m_Console->PushText(date + ": " + log + "\n",glm::vec3(1, 0.3, 0.1));

			std::cout << ANSI(ANSI_COLOR_RED_F) << "ERROR: " << date << ": " << fncname << ": " << log << ANSI(ANSI_COLOR_WHITE_F) << std::endl;

			m_LogFile.flush();
			va_end(args);
			delete[] log;
		}
	}
	

	void Logger::ConnectConsole(Ref<Console> console)
	{
		m_Console = console;
	}

	void Logger::SetLogFile(const std::string& logfile)
	{
		m_FileName = logfile;
	}
	std::shared_ptr<Logger> Logger::Get()
	{
		return m_This;
	}
	
	std::ostream& operator<<(std::ostream& os, ANSI ansi)
	{
		printf("\033[1;%dm", ansi.m_Code);
		return os;
	}
	

}
