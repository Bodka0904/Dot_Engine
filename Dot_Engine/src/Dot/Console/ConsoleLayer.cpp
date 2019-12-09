#include "stdafx.h"
#include "ConsoleLayer.h"
#include "Dot/Input.h"
#include "Dot/MouseButtonCodes.h"

namespace Dot {
	ConsoleLayer::ConsoleLayer()
	{
		
	}
	ConsoleLayer::~ConsoleLayer()
	{
	}
	void ConsoleLayer::OnAttach()
	{
		std::pair<int, int> winSize = Input::GetWindowSize();
		m_Camera = std::make_shared<OrthoCamera>(0, winSize.first, winSize.second, 0);

		m_Shader = std::make_shared<Shader>("ConsoleShader", "res/shaders/Dot/ConsoleShader.glsl");
		m_Shader->AddUniform("u_ViewProjectionMatrix");

		m_TextShader = std::make_shared<Shader>("TextShader", "res/shaders/Text/TextShader.glsl");
		m_TextShader->AddUniform("u_ViewProjectionMatrix");
		m_TextShader->AddUniform("u_Color");
		m_TextShader->AddUniform("u_Texture");
		m_TextShader->UploadUniformInt("u_Texture", 0);

		m_Renderer = std::make_shared<Renderer2D>(1);
		m_Console = std::make_shared<Console>(glm::vec2(500, 500), glm::vec2(500, 500), 400, "Arial", glm::vec2(0.2, 0.2));
	
		m_Renderer->ClearBuffer();
		m_Renderer->Push(m_Console);
		m_Renderer->PrepareForRender();
	}
	void ConsoleLayer::OnUpdate(Timestep ts)
	{
		if (m_Console->Update())
		{
			m_Renderer->ClearBuffer();
			m_Renderer->Push(m_Console);
			m_Renderer->PrepareForRender();
		}
		m_Renderer->BeginScene(m_Shader, m_Camera);
		{
			m_Renderer->Render();
		}
		m_Renderer->EndScene();
	}
	void ConsoleLayer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& e = (WindowResizeEvent&)event;
			m_Camera->SetProjectionMatrix(0, e.GetWidth(), e.GetHeight(), 0);
		
		}
		else if (event.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressEvent& e = (MouseButtonPressEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{
				if (m_Console->HandleLeftClick())
				{
					e.IsHandled() = true;
				}

			}
			else if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				if (m_Console->HandleRighClick())
				{
					e.IsHandled() = true;
				}
			}
			
		}
		else if (event.GetEventType() == EventType::MouseButtonReleased)
		{
			MouseButtonReleaseEvent& e = (MouseButtonReleaseEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{
				m_Console->HandleRelease();
			}
			else if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				m_Console->HandleRelease();
			}
		}
	}
}