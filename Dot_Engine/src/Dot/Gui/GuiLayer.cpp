#include "stdafx.h"
#include "GuiLayer.h"
#include "Dot/Core/Application.h"

#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Core/Input.h"

#include "Dot/Gui/GuiSystem/Button.h"
#include "Dot/Gui/GuiSystem/Checkbox.h"
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/Gui.h"
#include "Dot/Gui/GuiSystem/GuiRenderer.h"

#include "Dot/Utils/Text/TextRenderer.h"

namespace Dot {
	GuiLayer::GuiLayer()
	{
	}

	GuiLayer::~GuiLayer()
	{
	}

	void GuiLayer::OnAttach()
	{
		m_Camera = std::make_shared<OrthoCamera>(0, Input::GetWindowSize().first, Input::GetWindowSize().second, 0);

		Wrapper::Create("wrapper", glm::vec2(300, 50), glm::vec2(300, 500));
		Gui::EnableWrapper("wrapper");
		{
			Button::Create("button", glm::vec2(300, 50), glm::vec2(50, 50));
			Checkbox::Create("checkbox", glm::vec2(300, 50), glm::vec2(50, 50));
			Slider::Create("slider", glm::vec2(50, 50), glm::vec2(200, 20), &value);
			Button::Create("button2", glm::vec2(300, 50), glm::vec2(50, 50));	
				
		}Gui::DisableWrapper();
		
		Text::Init();
		Gui::Init("res/textures/Dot/Gui/DefaultTexturePack/TexturePack_black.png");
		
		// GuiShader setup //
		m_GuiShader = std::make_shared<Shader>("GuiShader", "res/shaders/Dot/GuiShader.glsl");
		m_GuiShader->AddUniform("u_ViewProjectionMatrix");
		m_GuiShader->AddUniform("u_MousePos");
		m_GuiShader->AddUniform("u_Texture");
		m_GuiShader->UploadUniformInt("u_Texture", 0);
		/////////////////////
		// TextShader setup //
		m_TextShader = std::make_shared<Shader>("TextShader", "res/shaders/Text/TextShader.glsl");
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
		if (Button::GetWrapped("wrapper", "button").GetClicked())
		{
			std::cout << "Click!" << std::endl;
			m_Text->Update(" AKra");
			m_Renderer->PushOff(m_Text->GetVertice(5),5);
			//m_Renderer->ClearBuffer();
			//m_Renderer->Push(m_Text->GetVertice(0), m_Text->GetNumChar());
			//m_Renderer->PrepareForRender();
		}
		if (Checkbox::GetWrapped("wrapper", "checkbox").GetClicked())
		{
			std::cout << "Checked!" << std::endl;
		}
		if (Button::GetWrapped("wrapper", "button2").GetClicked())
		{
			m_Text->RestartCurserX();
			m_Text->SetPositionInBuffer(0);
		}
		
		Gui::Update();
		GuiRenderer::BeginRender(m_Camera);
		{
			m_GuiShader->Bind();
			m_GuiShader->UploadUniformFloat2("u_MousePos", glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
			Gui::BindTexture();
			GuiRenderer::Render(m_GuiShader, Gui::GetVAO());
		}
		GuiRenderer::EndRender();
		TextRenderer::BeginRender(m_Camera);
		{
			Font::Bind("Arial");
			m_TextShader->Bind();
			m_TextShader->UploadUniformFloat("u_Color", 1);
			TextRenderer::Render(m_TextShader, Text::GetVAO());
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
				if (Gui::HandleLeftClick())
				{
					e.IsHandled() = true;
				}
			
			}
			else if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				if (Gui::HandleRightClick())
				{
					e.IsHandled() = true;
				}
			}
			
		}
		else if (event.GetEventType() == EventType::MouseButtonReleased)
		{
			MouseButtonReleaseEvent& e = (MouseButtonReleaseEvent&)event;
			//if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				Gui::HandleRelease();
			}
			
		}

	}

}