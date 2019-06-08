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
	


	Gui::AddButton(BIND_FN(ButtonActions),"test",glm::vec3(COLOR_BLACK));
	Gui::AddButton(BIND_FN(ButtonActions),"test2",glm::vec3(COLOR_PURPLE));

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

	if (Gui::GetButton(1)->Clicked())
	{
		LOG_INFO("KRAVA")
	}
}
