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
	


	Gui::AddButton(BIND_FN(ButtonActions));
	Gui::AddButton(BIND_FN(ButtonActions));

	Gui::Init(app.GetWin().GetWindow());
	

}

void GuiLayer::OnUpdate()
{
	Gui::Update();
	Gui::Render();
}

void GuiLayer::ButtonActions()
{
	if (Gui::GetButton(0)->Clicked())
	{
		LOG_INFO("HHH")
	}
}
