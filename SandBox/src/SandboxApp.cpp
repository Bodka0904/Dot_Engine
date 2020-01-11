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
		PushLayer(new Dot::ParticleEditorLayer());
	}

	~Sandbox()
	{
	}
};


Dot::Application* CreateApplication()
{
	return new Sandbox();
}