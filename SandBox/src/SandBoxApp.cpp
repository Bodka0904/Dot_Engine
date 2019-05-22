#pragma once
#include "Dot/Application.h"
#include "Dot/EntryPoint.h"
#include <iostream>



class SandBox : public Application
{
public:
	SandBox()
	{

	}

	~SandBox()
	{

	}

private:
	
};


class TestLayer : public Layer
{
public:
	TestLayer() 
	{
		
	};
	void OnAttach()
	{
		
	}

	void OnUpdate()
	{
		
		
	}


	
private:

	
};



Application* CreateApplication()
{
	return new SandBox();
}





