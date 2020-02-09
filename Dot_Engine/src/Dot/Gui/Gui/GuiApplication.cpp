#include "stdafx.h"
#include "GuiApplication.h"

#include "Dot/Core/Input.h"
#include "Dot/Renderer/RenderCommand.h"
#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Renderer/Renderer.h"

namespace Dot {
	GuiApplication *GuiApplication::s_Instance;
	void GuiApplication::Init(const std::string& texturePath)
	{
		if (!s_Instance)
		{
			s_Instance = new GuiApplication(texturePath);
		}
	}
	GuiApplication::GuiApplication(const std::string& texturePath)	
	{	
		glm::vec2 winSize = glm::vec2(float(Input::GetWindowSize().first), float(Input::GetWindowSize().second));
		m_Texture = Texture2D::Create(texturePath, true,false);
		m_GuiShader = Shader::Create("GuiShader", "res/Shaders/Dot/GuiShader.glsl");
		m_Camera = std::make_shared<OrthoCamera>(0.0f, float(Input::GetWindowSize().first), float(Input::GetWindowSize().second), 0.0f);


		glm::vec2 texCoords[4] = {
				glm::vec2(1.0f / 8.0f, 1.0f / 8.0f),
				glm::vec2(2.0f / 8.0f, 1.0f / 8.0f),
				glm::vec2(2.0f / 8.0f, 2.0f / 8.0f),
				glm::vec2(1.0f / 8.0f, 2.0f / 8.0f)
		};
		m_BackGroundQuad = QuadVertex2D(glm::vec2(0,0), glm::vec2(float(Input::GetWindowSize().first), float(Input::GetWindowSize().second)), glm::vec3(1, 1, 1), &texCoords[0]);
		m_Layers[BACK].guiRenderer->PushOffset(&m_BackGroundQuad, 1,0);
		PopIndex(BACK);
	}
	GuiApplication::~GuiApplication()
	{
		
	}
	void GuiApplication::Update()
	{
		if (m_CurrentBlock)
		{
			RenderCommand::Disable(D_DEPTH_TEST);
			m_GuiShader->Bind();
			m_GuiShader->UploadUniformMat4("u_ViewProjectionMatrix", m_Camera->GetViewProjectionMatrix());
			m_Texture->Bind(0);
			m_Layers[BACK].guiRenderer->Render();

			glm::vec2 mousePos = glm::vec2(Input::GetMousePosition().first, Input::GetMousePosition().second);

			m_CurrentBlock->HandleResize(mousePos);
			m_CurrentBlock->OnUpdate();

			for (auto& win : m_CurrentBlock->m_Window)
			{
				win.second->Update(mousePos);
				win.second->Render();
			}


			Font::Bind("Arial");

			for (auto& console : m_CurrentBlock->m_Console)
				console.second->Render();

			m_Layers[BACK].labelRenderer->Render();
			m_Layers[BACK].textRenderer->Render();


			m_Texture->Bind(0);
			m_Layers[FRONT].guiRenderer->Render();
			Font::Bind("Arial");
			m_Layers[FRONT].labelRenderer->Render();
			m_Layers[FRONT].textRenderer->Render();

			RenderCommand::Enable(D_DEPTH_TEST);
		}
	}

	unsigned int GuiApplication::AddBlock(Ref<GuiBlock> block)
	{
		m_Blocks[m_InsertedBlockID] = block;
		m_InsertedBlockID++;

		return m_InsertedBlockID - 1;
	}

	void GuiApplication::SwitchBlock(unsigned int ID)
	{
		auto it = m_Blocks.find(ID);
		if (it != m_Blocks.end())
		{
			if (m_CurrentBlock)
			{
				m_CurrentBlock->OnDetach();
			}
			m_CurrentBlock = it->second;
			m_CurrentBlock->OnAttach();
		}
	}
	
	void GuiApplication::OnEvent(Event& e)
	{
		if (m_CurrentBlock)
		{
			glm::vec2 mousePos = glm::vec2(Input::GetMousePosition().first, Input::GetMousePosition().second);
			if (e.GetEventType() == EventType::WindowResized)
			{
				WindowResizeEvent& event = (WindowResizeEvent&)e;
				m_Camera->SetProjectionMatrix(0, float(event.GetWidth()), float(event.GetHeight()), 0);
				m_WindowSize.x = float(event.GetWidth());
				m_WindowSize.y = float(event.GetHeight());

				m_CurrentBlock->OnWindowResize();
				for (auto& win : m_CurrentBlock->m_Window)
				{
					win.second->OnWindowResize(event);
				}
			}
			else if (e.GetEventType() == EventType::MouseButtonPressed)
			{
				MouseButtonPressEvent& event = (MouseButtonPressEvent&)e;
				if (event.GetButton() == D_MOUSE_BUTTON_LEFT)
				{
					if (m_CurrentBlock->OnLeftClick(mousePos))
						e.IsHandled() = true;
				}
				else if (event.GetButton() == D_MOUSE_BUTTON_RIGHT)
				{

				}
			}
			else if (e.GetEventType() == EventType::MouseButtonReleased)
			{
				MouseButtonReleaseEvent& event = (MouseButtonReleaseEvent&)e;
				if (event.GetButton() == D_MOUSE_BUTTON_LEFT)
				{
					if (m_CurrentBlock->OnLeftRelease())
						e.IsHandled() = true;
				}
			}
			else if (e.GetEventType() == EventType::KeyPressed)
			{
				Dot::KeyPressedEvent& event = (Dot::KeyPressedEvent&)e;

				for (auto& console : m_CurrentBlock->m_Console)
				{
					if (console.second->TakeInput(event))
					{
						e.IsHandled() = true;
						break;
					}
				}
			}
			m_CurrentBlock->OnEvent(e);
		}
	}


	void GuiApplication::PushIndex(int index, int layer)
	{
		if (s_Instance != NULL)
		{
			D_ASSERT(layer < NUM_GUI_LAYERS, "");
			D_ASSERT(index < MAX_QUADS, "");
			m_Layers[layer].availableIndex.push(index);
			m_Layers[layer].numQuads--;
		}
	}

	

	int GuiApplication::PopIndex(int layer)
	{
		D_ASSERT(layer < NUM_GUI_LAYERS, "");
		if (m_Layers[layer].numQuads < MAX_QUADS)
		{
			int index = m_Layers[layer].availableIndex.top();
			m_Layers[layer].availableIndex.pop();
			m_Layers[layer].numQuads++;
			return index;
		}
		return 0;
	}



	void GuiApplication::UpdateVertexBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len, int layer)
	{
		D_ASSERT(layer < NUM_GUI_LAYERS, "");
		m_Layers[layer].guiRenderer->PushOffset(vertices, len, index);
	}

	void GuiApplication::UpdateTextBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len, int layer)
	{
		D_ASSERT(layer < NUM_GUI_LAYERS, "");
		m_Layers[layer].textRenderer->PushOffset(&vertices[0], len, index * MAX_TEXT_CHAR);
	}

	void GuiApplication::UpdateLabelBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len, int layer)
	{
		D_ASSERT(layer < NUM_GUI_LAYERS, "");
		m_Layers[layer].labelRenderer->PushOffset(&vertices[0], len, index * MAX_CHAR_PER_LABEL);
	}
}