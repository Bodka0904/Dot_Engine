#include "stdafx.h"
#include "GuiLayer.h"
#include "Dot/Core/Application.h"

#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Core/Input.h"

#include "Dot/Gui/GuiSystem/Button.h"
#include "Dot/Gui/GuiSystem/Checkbox.h"
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/Gui.h"

#include "Dot/Renderer/TextRenderer.h"

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
		StaticText::Init(1000);
		Gui::Init("res/Textures/Gui/TexturePack_black.png");

		m_Camera = std::make_shared<OrthoCamera>(0, Input::GetWindowSize().first, Input::GetWindowSize().second, 0);	
		
		// GuiShader setup //
		m_GuiShader = Shader::Create("GuiShader", "res/Shaders/Dot/GuiShader.glsl");
		m_GuiShader->AddUniform("u_ViewProjectionMatrix");
		m_GuiShader->AddUniform("u_MousePos");
		m_GuiShader->AddUniform("u_Texture");
		m_GuiShader->UploadUniformInt("u_Texture", 0);
		/////////////////////
		// TextShader setup //
		m_TextShader = Shader::Create("TextShader", "res/Shaders/Text/TextShader.glsl");
		m_TextShader->AddUniform("u_ViewProjectionMatrix");
		m_TextShader->AddUniform("u_Color");
		m_TextShader->AddUniform("u_Texture");
		m_TextShader->UploadUniformInt("u_Texture", 0);
		/////////////////////

		m_Text = std::make_shared<DynamicText>("Arial", "Test", glm::vec2(300, 300), glm::vec2(0.2, 0.2), 100);
		m_Renderer = std::make_shared<Renderer2D>(5000);

		m_Renderer->ClearBuffer();
		m_Renderer->Push(m_Text->GetVertice(0), m_Text->GetNumChar());
		m_Renderer->PrepareForRender();
	}

	void GuiLayer::OnUpdate(Timestep ts)
	{	
		Gui::Get()->Update();
		Gui::Get()->Render(m_GuiShader, m_Camera);
		
		TextRenderer::BeginRender(m_Camera);
		{
			Font::Bind("Arial");
			m_TextShader->Bind();
			m_TextShader->UploadUniformFloat("u_Color", 1);
			TextRenderer::Render(m_TextShader, StaticText::GetVAO(), StaticText::GetCount()*4);
		}
		TextRenderer::EndRender();
		
		m_Renderer->BeginScene(m_TextShader, m_Camera);
		{
			m_TextShader->UploadUniformFloat("u_Color", 1);
			m_Renderer->Render();

		}m_Renderer->EndScene();
	}

	void GuiLayer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& e = (WindowResizeEvent&)event;
			m_Camera->SetProjectionMatrix(0, e.GetWidth(),e.GetHeight(), 0);
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

	}

}