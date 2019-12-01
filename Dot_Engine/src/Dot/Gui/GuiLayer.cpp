#include "stdafx.h"
#include "GuiLayer.h"
#include "Dot/Application.h"

#include "Dot/MouseButtonCodes.h"

#include "Dot/Graphics/GuiSystem/DButton.h"
#include "Dot/Graphics/GuiSystem/DCheckbox.h"
#include "Dot/Graphics/GuiSystem/DSlider.h"
#include "Dot/Graphics/GuiSystem/DGui.h"



namespace Dot {
	GuiLayer::GuiLayer()
	{
	}

	GuiLayer::~GuiLayer()
	{
	}

	void GuiLayer::OnAttach()
	{
		DWrapper::Create("wrapper", glm::vec2(300, 50), glm::vec2(300, 500));
		DGui::EnableWrapper("wrapper");
		{
			DButton::Create("button", glm::vec2(300, 50), glm::vec2(50, 50));
			DCheckbox::Create("checkbox", glm::vec2(300, 50), glm::vec2(50, 50));
			DSlider::Create("slider", glm::vec2(50, 50), glm::vec2(200, 20), &value);
		
		}DGui::DisableWrapper();
		
		DGui::Init("res/textures/Dot/Gui/DefaultTexturePack/DefaultTexturePack.png", "res/shaders/Dot/GuiShader.glsl", "res/shaders/Text/TextShader.glsl");
	}

	void GuiLayer::OnUpdate(Timestep ts)
	{
		if (DButton::GetWrapped("wrapper", "button").GetClicked())
		{
			std::cout << "Click!" << std::endl;
		}
		if (DCheckbox::GetWrapped("wrapper", "checkbox").GetClicked())
		{
			std::cout << "Checked!" << std::endl;
		}

		DGui::Update();
		DGui::Render();	
	}

	void GuiLayer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& e = (WindowResizeEvent&)event;
			DGui::UpdateCamera(glm::vec2(e.GetWidth(),e.GetHeight()));
		} 
		else if (event.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressEvent& e = (MouseButtonPressEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{	
				if (DGui::HandleLeftClick())
				{
					e.IsHandled() = true;
				}
			
			}
			else if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				if (DGui::HandleRightClick())
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
				DGui::HandleRelease();
			}
		}
	}

}