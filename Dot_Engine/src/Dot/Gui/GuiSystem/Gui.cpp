#include "stdafx.h"
#include "Gui.h"
#include "Dot/Core/Input.h"
#include "Dot/Debug/Log.h"

#include "Dot/Utils/Text/Font.h"
#include "Dot/Utils/Text/Text.h"

#define MAX_WIDGETS 300

namespace Dot {

	Gui* Gui::s_Instance = NULL;
	void Gui::Init(const std::string& texturePack)
	{
		if (!s_Instance)
		{
			s_Instance = new Gui(texturePack);
		}
	}
	Gui::Gui(const std::string& texturePack)
	{
		m_NumWidgets = 0;
		m_Locked = false;

		m_Texture = std::make_shared<Texture>(texturePack, true);
		m_GuiRenderer = std::make_shared<Renderer2D>(MAX_WIDGETS);

	}

	Gui::~Gui()
	{
		delete s_Instance;
	}

	
	bool Gui::HandleLeftClick()
	{
		m_MousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		for (auto& wrap : m_Wrapper)
		{
			if (wrap.second->Minimize(m_MousePosition))
			{
				return true;
			}
			if (!wrap.second->IsMinimized())
			{
				if (wrap.second->MouseResize(m_MousePosition))
				{
					m_ResizedWrapper = wrap.first;
					return true;
				}
				else
				{
					for (auto& widg : wrap.second->GetWidgets())
					{
						if (widg.second->MouseHoover(m_MousePosition))
						{
							widg.second->ClickHandle();
							return true;
						}
					}
				}
			}
		}
		for (auto& widg : m_Widget)
		{
			if (widg.second->MouseHoover(m_MousePosition))
			{
				widg.second->ClickHandle();
				return true;
			}
		}
		return false;
	}


	bool Gui::HandleRightClick()
	{
		m_MousePosition = glm::vec2(Input::GetMouseX(), Input::GetMouseY());
		for (auto& wrap : m_Wrapper)
		{
			if (!wrap.second->IsMinimized())
			{
				for (auto& widg : wrap.second->GetWidgets())
				{
					if (widg.second->MouseHoover(m_MousePosition))
					{
						m_AttachedWidget = widg.first;
						m_AttachedWrapper = wrap.first;
						return true;
					}
				}
				if (wrap.second->MouseHoover(m_MousePosition))
				{
					m_AttachedWrapper = wrap.first;
					return true;
				}
			}
		}
		for (auto& it : m_Widget)
		{
			if (it.second->MouseHoover(m_MousePosition))
			{
				m_AttachedWidget = it.first;
				return true;
			}
		}
		return false;
	}
	
	void Gui::HandleRelease()
	{
		m_AttachedWidget.clear();
		m_AttachedWrapper.clear();	
		m_ResizedWrapper.clear();
	}
	void Gui::UpdatePosBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len)
	{
		int counter = 0;
		for (int i = index; i < index + len; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_Vertices[i].m_Vertices[j].position = vertices[counter].m_Vertices[j].position;
			}
			counter++;
		}
		m_GuiRenderer->PushOffset(&m_Vertices[index], len,index);
	}
	void Gui::UpdateTexBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len)
	{
		int counter = 0;
		for (int i = index; i < index + len; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_Vertices[i].m_Vertices[j].texCoord = vertices[counter].m_Vertices[j].texCoord;
			}
			counter++;
		}
		m_GuiRenderer->PushOffset(&m_Vertices[index], len, index);
	}
	
	void Gui::Update()
	{
		glm::vec2 newMousePos;
		newMousePos.x = Input::GetMouseX();
		newMousePos.y = Input::GetMouseY();
		glm::vec2 dif = newMousePos - m_MousePosition;
		m_MousePosition = newMousePos;

		if (!m_AttachedWrapper.empty() && !m_AttachedWidget.empty())
		{
			m_Wrapper[m_AttachedWrapper]->GetWidget(m_AttachedWidget).Move(dif);
		}
		else if (!m_AttachedWidget.empty())
		{
			m_Widget[m_AttachedWidget]->Move(dif);
		}
		else if (!m_AttachedWrapper.empty())
		{
			m_Wrapper[m_AttachedWrapper]->Move(dif);
		}
		else if (!m_ResizedWrapper.empty())
		{
			m_Wrapper[m_ResizedWrapper]->Resize(m_MousePosition);
		}
	}

	void Gui::Render(const Ref<Shader> shader, const Ref<OrthoCamera> camera)
	{
		m_GuiRenderer->BeginScene(shader, camera);
		{
			m_Texture->Bind(0);
			shader->Bind();
			shader->UploadUniformFloat2("u_MousePos", glm::vec2(Input::GetMouseX(), Input::GetMouseY()));
			m_GuiRenderer->Render();
		}
		m_GuiRenderer->EndScene();
	}

	
	void Gui::AddWidget(const std::string& label, Ref<Widget> widget, const QuadVertex* quad, int num)
	{
		if (m_NumWidgets < MAX_WIDGETS)
		{
			if (!m_EnabledWrapper.empty())
			{
				m_Wrapper[m_EnabledWrapper]->AddWidget(label, widget, m_NumWidgets);
			}
			else
			{
				m_Widget[label] = widget;
				m_Widget[label]->SetIndex(m_NumWidgets);
			}

			for (int i = 0; i < num; ++i)
			{
				m_Vertices.push_back(quad[i]);
			}
			m_GuiRenderer->PushOffset(&m_Vertices[m_NumWidgets], 1, m_NumWidgets);
			m_NumWidgets++;
		}
		else
		{
			LOG_WARN("Reached maximum of widgets, not possible to add!");
		}
	}
	void Gui::AddWrapper(const std::string label, Ref<Wrapper> wrapper, const QuadVertex* quad, int num)
	{
		if (m_NumWidgets < MAX_WIDGETS)
		{
			wrapper->SetIndex(m_NumWidgets);
			m_Wrapper[label] = wrapper;
			for (int i = 0; i < num; ++i)
			{
				m_Vertices.push_back(quad[i]);
			}
			m_GuiRenderer->PushOffset(&m_Vertices[m_NumWidgets], 2, m_NumWidgets);
			m_NumWidgets += 2;
		}
		else
		{
			LOG_WARN("Reached maximum of widgets, not possible to add!");
		}
	}
	void Gui::EnableWrapper(const std::string& label)
	{
		m_EnabledWrapper = label;
	}
	void Gui::DisableWrapper()
	{
		m_Wrapper[m_EnabledWrapper]->SetWidgetPosition();
		m_EnabledWrapper.clear();
	}
	Wrapper::Wrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		m_Position(position),
		m_Size(size),
		m_Index(0),
		m_ExitButton(glm::vec2(position.x + size.x - 20,position.y),glm::vec2(20,20))
	{
	}
	void Wrapper::AddWidget(const std::string& label, Ref<Widget> widget, unsigned int index)
	{
		m_Widget[label] = widget;
		m_Widget[label]->SetIndex(index);
	}
	bool Wrapper::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	bool Wrapper::MouseResize(const glm::vec2& mousePos)
	{
		int offset = 20;
		if (mousePos.x < m_Position.x + m_Size.x && mousePos.x > m_Position.x + m_Size.x - offset // BOTTOM RIGHT CORNER
			&& mousePos.y < m_Position.y + m_Size.y && mousePos.y > m_Position.y + m_Size.y - offset)
		{
			return true;
		}
		return false;
	}
	
	bool Wrapper::Minimize(const glm::vec2& mousePos)
	{
		if (m_ExitButton.MouseHoover(mousePos))
		{
			if (!m_Minimized)
			{
				for (auto& it : m_Widget)
				{
					it.second->Minimize();
				}
				m_Minimized = true;
				Resize(m_Position + m_ExitButton.GetSize());
			}
			else
			{
				m_Minimized = false;
				Resize(m_Position + glm::vec2(100,100));
				SetWidgetPosition();
			}
			return true;
		}
		return false;
	}

	void Wrapper::Resize(const glm::vec2& mousePos)
	{
		m_Size = abs(mousePos - m_Position);
		m_ExitButton.SetPosition(glm::vec2(m_Position.x+m_Size.x- m_ExitButton.GetSize().x,m_Position.y));

		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index,&newVertex);
	}
	void Wrapper::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		if (!m_Minimized)
		{
			for (auto& it : m_Widget)
			{
				it.second->Move(pos);
			}
		}
		m_ExitButton.Move(pos);
		
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
	}
	void Wrapper::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		if (!m_Minimized)
		{
			for (auto& it : m_Widget)
			{
				glm::vec2 posDif = it.second->GetPosition() - m_Position;
				it.second->SetPosition(posDif);
			}
		}
		glm::vec2 posDif = m_ExitButton.GetPosition() - m_Position;
		m_ExitButton.SetPosition(posDif);

		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
	}
	void Wrapper::SetWidgetPosition()
	{
		glm::vec2 newPos = m_Position;
		for (auto& it : m_Widget)
		{
			newPos.y += it.second->GetLabelSize().y;
			it.second->SetPosition(newPos);
			newPos.y += it.second->GetSize().y;	
			if (m_Size.x < it.second->GetSize().x)
				m_Size.x = it.second->GetSize().x+20;
		}
		if (newPos.y > m_Size.y + m_Position.y)
		{
			m_Size.y = (newPos.y - m_Position.y)+20;
			m_ExitButton.SetPosition(glm::vec2(m_Position.x + m_Size.x - m_ExitButton.GetSize().x, m_Position.y));
			QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
			Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		}
	}
	void Wrapper::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[8] = {
			   glm::vec2(0, 0.75),
			   glm::vec2(0.25, 0.75),
			   glm::vec2(0.25, 1),
			   glm::vec2(0, 1),
			   // Exit button
			   glm::vec2(0.5, 0.75),
			   glm::vec2(0.75, 0.75),
			   glm::vec2(0.75, 1),
			   glm::vec2(0.5,  1)
		};
		QuadVertex quadVertex[2] = {
			QuadVertex(position,size,&texCoords[0]),
			QuadVertex(glm::vec2(position.x + size.x - 20,position.y),glm::vec2(20,20),&texCoords[4])
		};
		
		Ref<Wrapper> wrapper = std::make_shared<Wrapper>(label, position, size);
		Gui::Get()->AddWrapper(label, wrapper, &quadVertex[0]);
	}
	glm::vec4 Wrapper::GetCoords()
	{
		return glm::vec4(  m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
	Wrapper::ExitButton::ExitButton(const glm::vec2& position, const glm::vec2& size)
		:
		m_Position(position),m_Size(size),m_Index(0)
	{
	}
	bool Wrapper::ExitButton::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void Wrapper::ExitButton::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
	}
	void Wrapper::ExitButton::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
	}
	glm::vec4 Wrapper::ExitButton::GetCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}