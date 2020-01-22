#include "stdafx.h"
#include "GuiLayer.h"

#include "Dot/Gui/Gui/GuiApplication.h"

#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Dot {
	GuiLayer::GuiLayer()
	{
	}

	GuiLayer::~GuiLayer()
	{
	
	}

	void GuiLayer::OnAttach()
	{
		//Default font
		Font::AddFont("Arial", "res/Fonts/Arial/Arial.fnt", "res/Fonts/Arial/Arial.png");
		GuiApplication::Init("res/Textures/Gui/gui.png");
		
	}

	void GuiLayer::OnDetach()
	{

	}

	void GuiLayer::OnUpdate(Timestep ts)
	{
		GuiApplication::Get()->Update();
	}

	void GuiLayer::OnEvent(Event& event)
	{
		GuiApplication::Get()->OnEvent(event);
	}


}