#pragma once
#include <Dot.h>
#include <Dot/Core/EntryPoint.h>

#include "GameLayer.h"

class Sandbox : public Dot::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{
	}
};


Dot::Application* CreateApplication()
{
	return new Sandbox();
}