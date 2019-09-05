#pragma once
#include "Debug/Log.h"
#include "Application.h"


extern Dot::Application* CreateApplication();


int main(int argc, char** argv)
{
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);

	auto app = CreateApplication();
	app->Run();	
	delete app;

}

