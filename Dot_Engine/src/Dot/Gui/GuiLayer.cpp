#include "stdafx.h"
#include "GuiLayer.h"
#include "Dot/Application.h"



#define BIND_FN(x) std::bind(&GuiLayer::x ,this)

GuiLayer::GuiLayer()
{
}


GuiLayer::~GuiLayer()
{
}


void GuiLayer::OnAttach()
{
	Application& app = Application::Get();
	


	Gui::AddButton(BIND_FN(ButtonAction),"test");
	Gui::AddButton(BIND_FN(ButtonAction),"test2");
	Gui::AddCheckBox(BIND_FN(CheckBoxAction),"test3");
	Gui::AddSlider("HOLA");

	Gui::Init(app.GetWin().GetWindow(),glm::vec3(COLOR_BLACK));
	

}

void GuiLayer::OnUpdate()
{
	Gui::Update();
	Gui::Render();
}

void GuiLayer::ButtonAction()
{
	LOG_INFO("HHH")

	LOG_INFO("KRAVA")
	
	LOG_INFO("WORKING");
}

void GuiLayer::CheckBoxAction()
{
	LOG_WARN("ACTIVATED");
}
