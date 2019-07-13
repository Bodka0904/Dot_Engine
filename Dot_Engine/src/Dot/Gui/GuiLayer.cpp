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

		Gui::AddWrapper("MAIN",wrap.main_wrapper);
		Gui::GetWrapper(wrap.main_wrapper).AddButton(BIND_FN(ButtonAction), "button1");
		Gui::GetWrapper(wrap.main_wrapper).AddButton(BIND_FN(ButtonAction), "button2");
		//Gui::GetWrapper(wrap.main_wrapper).AddArrowButton(BIND_FN(ArrowButtonAction), "arrow btn");
		Gui::GetWrapper(wrap.main_wrapper).AddCheckBox(BIND_FN(CheckBoxAction), "checkbox");
		Gui::GetWrapper(wrap.main_wrapper).AddSlider("HAHA");

		Gui::AddWrapper("TEST",wrap.test_wrapper);
		Gui::GetWrapper(wrap.test_wrapper).AddSlider("HOHO");
		Gui::GetWrapper(wrap.test_wrapper).AddButton(BIND_FN(TestVisibleOff), "test off");
		Gui::GetWrapper(wrap.test_wrapper).AddButton(BIND_FN(TestVisibleOn), "test on");
		
	
		
	}

	void GuiLayer::OnUpdate()
	{

		Gui::Update();
		Gui::Render();
		
	}

	void GuiLayer::ButtonAction()
	{	
		LOG_INFO("HHH");

		LOG_INFO("KRAVA");

		LOG_INFO("WORKING");
	}

	void GuiLayer::CheckBoxAction()
	{
		LOG_WARN("ACTIVATED");
	}

	void GuiLayer::ArrowButtonAction()
	{
		//if (Gui::GetWrapper(0).GetArrowButton(0).LeftClicked())
		{
			LOG_INFO("KOLIBRIK");
		}
	}

	void GuiLayer::TestVisibleOff()
	{
		Gui::GetWrapper(0).GetVisible() = false;
	}

	void GuiLayer::TestVisibleOn()
	{
		Gui::GetWrapper(0).GetVisible() = true;
	}

}