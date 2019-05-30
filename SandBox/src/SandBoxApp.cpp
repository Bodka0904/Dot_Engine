#pragma once
#include "stdafx.h"
#include "Dot/Application.h"
#include "Dot/EntryPoint.h"
#include "Dot/KeyCodes.h"
#include "Dot/Events/Event.h"



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

	void OnUpdate() override
	{
		

	}

	void OnEvent(Event & event) override
	{
		if (event.GetEventType() == EventType::KeyPressed)
		{
			if (event.GetKeyCode() == D_KEY_TAB)
			{
				LOG_INFO("TAB PRESSED")
			}
			if (event.GetKeyCode() == D_KEY_LEFT)
			{
				LOG_INFO("LEFT PRESSED")
			}
		}
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





