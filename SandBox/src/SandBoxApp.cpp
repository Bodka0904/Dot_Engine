#pragma once
#include "Dot/Application.h"
#include "Dot/EntryPoint.h"
#include <iostream>



class TestLayer : public Layer
{
public:
	TestLayer()
		: Layer()
	{

	}

	void OnAttach()
	{
	
	}

	void OnUpdate()
	{
		

	}



private:
	

};


class SandBox : public Application
{
public:
	SandBox()
	{
		PushLayer(new TestLayer());
	}

	~SandBox()
	{

	}

private:
	
};




Application* CreateApplication()
{
	return new SandBox();
}





