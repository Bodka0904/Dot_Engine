#include "stdafx.h"
#include "GuiLayer.h"
#include "Dot/Core/Application.h"

#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Core/Input.h"

#include "Dot/Gui/GuiSystem/Button.h"
#include "Dot/Gui/GuiSystem/Checkbox.h"
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/TextArea.h"
#include "Dot/Gui/GuiSystem/Gui.h"

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
		Gui::Init("res/Textures/Gui/TexturePack_black.png");
		
		// GuiShader setup //
		m_GuiShader = Shader::Create("GuiShader", "res/Shaders/Dot/GuiShader.glsl");
		/////////////////////
		// TextShader setup //
		m_TextShader = Shader::Create("TextShader", "res/Shaders/Text/TextShader.glsl");
		/////////////////////
		m_Camera = std::make_shared<OrthoCamera>(0,Input::GetWindowSize().first,Input::GetWindowSize().second,0);
	
	}

	void GuiLayer::OnUpdate(Timestep ts)
	{
		Gui::Get()->Update();
		Gui::Get()->Render(m_GuiShader,m_TextShader, m_Camera);	
	}

	void GuiLayer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& e = (WindowResizeEvent&)event;
			m_Camera->SetProjectionMatrix(0.0f, (float)e.GetWidth(),(float)e.GetHeight(), 0.0f);
		} 
		else if (event.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressEvent& e = (MouseButtonPressEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{	
				if (Gui::Get()->HandleLeftClick())
				{
					e.IsHandled() = true;			
				}
			
			}
			else if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				if (Gui::Get()->HandleRightClick())
				{
					e.IsHandled() = true;			
				}
			}
			
		}
		else if (event.GetEventType() == EventType::MouseButtonReleased)
		{
			MouseButtonReleaseEvent& e = (MouseButtonReleaseEvent&)event;
			Gui::Get()->HandleRelease();	
		}
		else if (event.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& e = (KeyPressedEvent&)event;
		}

	}

}