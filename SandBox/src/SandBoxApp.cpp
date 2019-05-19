#include "Dot/Log.h"
#include "Dot/Application.h"
#include <iostream>

int main()
{

	LOG_INFO("test");
	std::cin.get();

	return 0;
}

class SandBox : public Application
{
public:
	SandBox()
	{

	}

	~SandBox()
	{

	}
};

Application* CreateApplication()
{
	return new SandBox();
}