#include "stdafx.h"
#include "GuiApplication.h"

#include "Dot/Core/Input.h"
#include "Dot/Renderer/RenderCommand.h"
#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Renderer/Renderer.h"

namespace Dot {
	GuiApplication* GuiApplication::s_Instance = NULL;
	void GuiApplication::Init(const std::string& texturePath)
	{
		if (!s_Instance)
		{
			s_Instance = new GuiApplication(texturePath);
		}
	}
	GuiApplication::GuiApplication(const std::string& texturePath)	
	{	
		m_WindowSize = Input::GetWindowSize();
		m_Texture = Texture2D::Create(texturePath, true,false);
		m_GuiShader = Shader::Create("GuiShader", "res/Shaders/Dot/GuiShader.glsl");
		m_Camera = std::make_shared<OrthoCamera>(0, Input::GetWindowSize().x, Input::GetWindowSize().y, 0);

		m_GuiRenderer = std::make_shared<Renderer2D>(MAX_QUADS);
		m_LabelRenderer = std::make_shared<Renderer2D>(MAX_QUADS * MAX_CHAR_PER_LABEL);
		m_TextRenderer = std::make_shared<Renderer2D>(MAX_QUADS * MAX_TEXT_CHAR);


		glm::vec2 texCoords[4] = {
				glm::vec2(1.0f / 8.0f, 1.0f / 8.0f),
				glm::vec2(2.0f / 8.0f, 1.0f / 8.0f),
				glm::vec2(2.0f / 8.0f, 2.0f / 8.0f),
				glm::vec2(1.0f / 8.0f, 2.0f / 8.0f)
		};
		m_BackGroundQuad = QuadVertex2D(glm::vec2(0,0),Input::GetWindowSize(), glm::vec3(1, 1, 1), &texCoords[0]);
		m_GuiRenderer->PushOffset(&m_BackGroundQuad, 1,0);
		for (int i = 1; i < MAX_QUADS; ++i)
		{
			m_AvailableIndex.push(i);
		}
	}
	GuiApplication::~GuiApplication()
	{
		for (GuiBlock* block : m_Blocks)
		{
			block->OnDetach();
			delete block;
		}
		delete s_Instance;
	}
	void GuiApplication::Update()
	{
		RenderCommand::Disable(D_DEPTH_TEST);
		m_GuiShader->Bind();
		m_GuiShader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera->GetViewProjectionMatrix());
		m_Texture->Bind(0);
		m_GuiRenderer->Render();
		
		glm::vec2 mousePos = glm::vec2(Input::GetMousePosition().first, Input::GetMousePosition().second);
		for (int i = 0; i < m_Blocks.size(); ++i)
		{
		
			m_Blocks[i]->HandleResize(mousePos);
			m_Blocks[i]->OnUpdate();
			for (auto& console : m_Blocks[i]->m_Console)
				console.second->Update(mousePos);
			for (auto& panel : m_Blocks[i]->m_Panel)
				panel.second->Update(mousePos);	
			for (auto& win : m_Blocks[i]->m_Window)
			{
				win.second->Update(mousePos);
				win.second->Render();
			}
		}


		Font::Bind("Arial");
		for (int i = 0; i < m_Blocks.size(); ++i)
		{
			for (auto& console : m_Blocks[i]->m_Console)
				console.second->Render();
		}
		m_LabelRenderer->Render();
		m_TextRenderer->Render();
		RenderCommand::Enable(D_DEPTH_TEST);
	}
	void GuiApplication::PushBlock(GuiBlock* block)
	{
		m_Blocks.emplace(m_Blocks.begin() + m_BlockInsertIndex, block);
		m_BlockInsertIndex++;
		block->OnAttach();
	}
	void GuiApplication::PopBlock(GuiBlock* block)
	{
		auto it = std::find(m_Blocks.begin(), m_Blocks.begin() + m_BlockInsertIndex, block);
		if (it != m_Blocks.begin() + m_BlockInsertIndex)
		{
			(*it)->OnDetach();
			m_Blocks.erase(it);
			m_BlockInsertIndex--;
		}
	}
	void GuiApplication::OnEvent(Event& e)
	{
		glm::vec2 mousePos = glm::vec2(Input::GetMousePosition().first, Input::GetMousePosition().second);
		if (e.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& event = (WindowResizeEvent&)e;
			m_Camera->SetProjectionMatrix(0, event.GetWidth(), event.GetHeight(), 0);
			m_WindowSize.x = event.GetWidth();
			m_WindowSize.y = event.GetHeight();
			for (int i = 0; i < m_Blocks.size(); ++i)
			{
			
				for (auto& win : m_Blocks[i]->m_Window)
				{
					win.second->OnWindowResize(event);
				}
			}
		}
		else if (e.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressEvent& event = (MouseButtonPressEvent&)e;
			if (event.GetButton() == D_MOUSE_BUTTON_LEFT)
			{
				for (int i = 0; i < m_Blocks.size(); ++i)
				{
					if (m_Blocks[i]->OnLeftClick(mousePos))
						break;
				}
			}
			else if (event.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				for (int i = 0; i < m_Blocks.size(); ++i)
				{
					m_Blocks[i]->OnRightClick();
				}
			}
		}
		else if (e.GetEventType() == EventType::MouseButtonReleased)
		{
			MouseButtonReleaseEvent& event = (MouseButtonReleaseEvent&)e;
			if (event.GetButton() == D_MOUSE_BUTTON_LEFT)
			{
				for (int i = 0; i < m_Blocks.size(); ++i)
				{
					m_Blocks[i]->OnLeftRelease();
				}
			}
		}
		else if (e.GetEventType() == EventType::KeyPressed)
		{
			Dot::KeyPressedEvent& event = (Dot::KeyPressedEvent&)e;
			for (int i = 0; i < m_Blocks.size(); ++i)
			{
				for (auto& console : m_Blocks[i]->m_Console)
				{
					if (console.second->TakeInput(event))
					{
						e.IsHandled() = true;
						break;
					}
				}
			}
		}
		for (int i = 0; i < m_Blocks.size(); ++i)
		{
			m_Blocks[i]->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}


	void GuiApplication::PushIndex(unsigned int index)
	{
		m_AvailableIndex.push(index);
		m_NumQuads--;
	}

	unsigned int GuiApplication::PopIndex()
	{
		if (m_NumQuads < MAX_QUADS)
		{
			m_NumQuads++;
			unsigned int index = m_AvailableIndex.top();
			m_AvailableIndex.pop();
			return index;
		}
		return 0;
	}

	void GuiApplication::UpdateVertexBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len)
	{
		m_GuiRenderer->PushOffset(vertices, len, index);
	}

	void GuiApplication::UpdateTextBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len)
	{
		m_TextRenderer->PushOffset(&vertices[0], len, index * MAX_TEXT_CHAR);
	}

	void GuiApplication::UpdateLabelBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len)
	{
		m_LabelRenderer->PushOffset(&vertices[0], len, index * MAX_CHAR_PER_LABEL);
	}

	
	
}