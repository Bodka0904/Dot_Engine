#include "Log.h"


inline void Log::SetLevel(unsigned int Level)
{
	LEVEL = Level;
}


inline void Log::SetColor(unsigned int color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

inline void Log::Message(std::string caller, std::string msg)
{
	std::cout << caller<<": "<< msg << std::endl;
}
