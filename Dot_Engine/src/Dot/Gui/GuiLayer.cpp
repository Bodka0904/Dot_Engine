#include "stdafx.h"
#include "GuiLayer.h"
#include "Dot/Application.h"
#include "Dot_GUI/src/Gui.h"


namespace Dot {
#define BIND_FN(x) std::bind(&GuiLayer::x ,this)

	GuiLayer::GuiLayer()
	{
	}


	GuiLayer::~GuiLayer()
	{
		Gui::Clear();
	}

	void GuiLayer::OnAttach()
	{
	
		Gui::AddWrapper("MAIN");
		Gui::GetWrapper("MAIN").AddButton(BIND_FN(ButtonAction), "button1");
		Gui::GetWrapper("MAIN").AddButton(BIND_FN(ButtonAction), "button2");
		Gui::GetWrapper("MAIN").AddArrowButton(BIND_FN(ArrowButtonAction), "arrow btn");
		Gui::GetWrapper("MAIN").AddArrowButton(BIND_FN(ButtonAction), "arrow btn2");
		Gui::GetWrapper("MAIN").AddCheckBox(BIND_FN(CheckBoxAction), "checkbox");
		Gui::GetWrapper("MAIN").AddSlider("HAHA");
		

		Gui::AddWrapper("TEST");
		Gui::GetWrapper("TEST").AddSlider("HOHO");
		Gui::GetWrapper("TEST").AddButton(BIND_FN(TestVisibleOff), "test off");
		Gui::GetWrapper("TEST").AddButton(BIND_FN(TestVisibleOn), "test on");
	
	}

	void GuiLayer::OnUpdate(Timestep ts)
	{

		Gui::Update();
		Gui::Render();
		
	}

	void GuiLayer::OnEvent(Event& event)
	{
		if (Gui::IsHandlingEvent())
		{
			event.IsHandled() = true;
			Gui::IsHandlingEvent() = false;
		}
	}

	void GuiLayer::ButtonAction()
	{	
		LOG_INFO("%f",Gui::GetWrapper("TEST").GetSlider(0).GetValue())
	}

	void GuiLayer::CheckBoxAction()
	{
		LOG_WARN("ACTIVATED");
	}

	void GuiLayer::ArrowButtonAction()
	{
		if (Gui::GetWrapper("TEST").GetArrowButton(0).LeftClicked())
		{
			LOG_INFO("KOLIBRIK");
		}
	}

	void GuiLayer::TestVisibleOff()
	{
		Gui::GetWrapper("MAIN").GetVisible() = false;
	}

	void GuiLayer::TestVisibleOn()
	{
		Gui::GetWrapper("MAIN").GetVisible() = true;
	}

}