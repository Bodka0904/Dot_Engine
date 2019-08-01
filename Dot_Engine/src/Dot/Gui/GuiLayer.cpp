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
		Application& app = Application::Get();

		main = Gui::AddWrapper("MAIN");
		Gui::GetWrapper(main).AddButton(BIND_FN(ButtonAction), "button1");
		Gui::GetWrapper(main).AddButton(BIND_FN(ButtonAction), "button2");
		Gui::GetWrapper(main).AddArrowButton(BIND_FN(ArrowButtonAction), "arrow btn");
		Gui::GetWrapper(main).AddArrowButton(BIND_FN(ButtonAction), "arrow btn2");
		Gui::GetWrapper(main).AddCheckBox(BIND_FN(CheckBoxAction), "checkbox");
		Gui::GetWrapper(main).AddSlider("HAHA");

		test = Gui::AddWrapper("TEST");
		Gui::GetWrapper(test).AddSlider("HOHO");
		Gui::GetWrapper(test).AddButton(BIND_FN(TestVisibleOff), "test off");
		Gui::GetWrapper(test).AddButton(BIND_FN(TestVisibleOn), "test on");
		
	
		
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
		LOG_INFO("%f",Gui::GetWrapper(test).GetSlider(0).GetValue())
	}

	void GuiLayer::CheckBoxAction()
	{
		LOG_WARN("ACTIVATED");
	}

	void GuiLayer::ArrowButtonAction()
	{
		if (Gui::GetWrapper(test).GetArrowButton(0).LeftClicked())
		{
			LOG_INFO("KOLIBRIK");
		}
	}

	void GuiLayer::TestVisibleOff()
	{
		Gui::GetWrapper(main).GetVisible() = false;
	}

	void GuiLayer::TestVisibleOn()
	{
		Gui::GetWrapper(main).GetVisible() = true;
	}

}