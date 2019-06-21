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
	
	Gui::AddButton(BIND_FN(ButtonAction),"button1");
	Gui::AddButton(BIND_FN(ButtonAction),"button2");
	Gui::AddArrowButton(BIND_FN(ArrowButtonAction), "arrow btn");
	Gui::AddCheckBox(BIND_FN(CheckBoxAction),"checkbox");
	Gui::AddWrapper();

	Gui::Init(app.GetWin().GetWindow());
	

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

void GuiLayer::ArrowButtonAction()
{
	if (Gui::GetArrowButton(0).LeftClicked())
	{
		LOG_INFO("KOLIBRIK");
	}
}
