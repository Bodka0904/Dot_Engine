#pragma once
#include "Dot/Debug/Log.h"
#include "Application.h"


extern Dot::Application* CreateApplication();


int main(int argc, char** argv)
{
	Dot::Logger::Init();
	auto app = CreateApplication();
	app->Run();	
	delete app;
}

