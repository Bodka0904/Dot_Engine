#include "stdafx.h"
#include "GuiLayer.h"
#include "Dot/Application.h"
#include "Dot/Graphics/Gui/Button.h"
#include "Dot/Graphics/Gui/CheckBox.h"
#include "Dot/Graphics/Gui/ArrowButton.h"
#include "Dot/Graphics/Gui/Slider.h"

#include "Dot/MouseButtonCodes.h"


namespace Dot {
	GuiLayer::GuiLayer()
	{
	}

	GuiLayer::~GuiLayer()
	{
	}

	void GuiLayer::OnAttach()
	{
		Wrapper::Create("wrapper", glm::vec2(300, 500), glm::vec2(300, 300));
		Gui::EnableWrapper("wrapper");
		{
			Button::Create("button", glm::vec2(300, 50), glm::vec2(50, 50));
			CheckBox::Create("checkbox", glm::vec2(50, 50), glm::vec2(50, 50));
			ArrowButton::Create("arrowbutton", glm::vec2(50, 50), glm::vec2(60, 50));
			Slider::Create("slider", glm::vec2(50, 50), glm::vec2(200, 20), &value);

		}Gui::DisableWrapper();

		m_Stack = std::make_shared<Gui>("res/shaders/Dot/GuiShader.glsl", "res/shaders/Text/TextShader.glsl", "res/textures/Dot/Gui/DefaultTexturePack/DefaultTexturePack.png");

	}

	void GuiLayer::OnUpdate(Timestep ts)
	{
		//if (Gui::GetWidget("button")->GetClicked())
		//{
		//	std::cout << "Clicked!" << std::endl;
		//	std::cout << value << std::endl;
		//}
		//if (Gui::GetWidget("checkbox")->GetClicked())
		//{
		//	std::cout << "Checked!" << std::endl;
		//}
		//if (Gui::GetWidget("arrowbutton")->LeftClicked())
		//{
		//	std::cout << "Left Clicked!" << std::endl;
		//}

		m_Stack->Update();	
	
		m_Stack->RenderWidgets();
		m_Stack->RenderLabels();
	}

	void GuiLayer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& e = (WindowResizeEvent&)event;
			m_Stack->UpdateCamera(e.GetWidth(), e.GetHeight());
		
		} 
		else if (event.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressEvent& e = (MouseButtonPressEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{	
				if (m_Stack->HandleLeftClick())
				{
					e.IsHandled() = true;
				}
			
			}
			else if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				if (m_Stack->HandleRightClick())
				{
					e.IsHandled() = true;
				}
			}
		}
		else if (event.GetEventType() == EventType::MouseButtonReleased)
		{
			MouseButtonReleaseEvent& e = (MouseButtonReleaseEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				m_Stack->Release();
			}
		}
	}

}