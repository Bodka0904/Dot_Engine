#include "stdafx.h"
#include "Dropdown.h"
#include "Dot/Gui/Gui/GuiApplication.h"

namespace Dot {
	Dropdown::Dropdown(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
		:
		m_Position(position),
		m_Size(size),
		m_Minimized(true),
		m_CheckedBox(0)
	{
		glm::vec2 texCoords[4] = {
				   glm::vec2(0.0f,		 0.0f),
				   glm::vec2(1.0f / 8.0f,0.0f),
				   glm::vec2(1.0f / 8.0f,1.0f / 8.0f),
				   glm::vec2(0.0f,		 1.0f / 8.0f)
		};

		QuadVertex2D quad(m_Position, m_Size, glm::vec3(1, 1, 1), &texCoords[0]);
		Dropbox box{ quad,Text("Arial", label, glm::vec2(position.x + m_Size.x + 3, position.y + m_Size.y / 4.0f), glm::vec2(0.18f), glm::vec3(0.2, 1, 0.5), MAX_TEXT_CHAR),
		GuiApplication::Get()->PopIndex(GuiApplication::FRONT) };

		m_Box.push_back(box);
		updateBuffers();
	}
	Dropdown::~Dropdown()
	{
		Clean();
	}

	void Dropdown::Clean()
	{
		std::vector<QuadVertex2D> quad;
		quad.resize(MAX_TEXT_CHAR);
		for (int i = 0; i < m_Box.size(); ++i)
		{
			GuiApplication::Get()->PushIndex(m_Box[i].index, GuiApplication::FRONT);
			GuiApplication::Get()->UpdateVertexBuffer(m_Box[i].index, &QuadVertex2D(), 1, GuiApplication::FRONT);
			GuiApplication::Get()->UpdateTextBuffer(m_Box[i].index, &quad[0], MAX_TEXT_CHAR, GuiApplication::FRONT);
		}
	}
	
	bool Dropdown::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (!m_Minimized)
			if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y + (m_Box.size() * m_Size.y) && mousePos.y >= coords.w)
			{
				return true;
			}
		if (m_Minimized)
			if (mousePos.x >= coords.x && mousePos.x <= coords.z
				&& mousePos.y <= coords.y && mousePos.y >= coords.w)
			{
				return true;
			}
		return false;
	}
	bool Dropdown::OnLeftClick(const glm::vec2& mousePos)
	{
		if (MouseHoover(mousePos))
		{
			int newCheckedBox = (int)floor((mousePos.y - m_Position.y) / m_Size.y);
			if (newCheckedBox == 0)
			{
				m_Minimized = !m_Minimized;
				minimize();
			}
			else if (newCheckedBox < m_Box.size())
			{
				m_Box[(size_t)m_CheckedBox].quad.SetColor(glm::vec3(0, 0, 0));
				if (newCheckedBox != m_CheckedBox)
				{
					m_CheckedBox = newCheckedBox;
					m_Minimized = false;
					m_Box[(size_t)m_CheckedBox].quad.SetColor(glm::vec3(0.3, 0.3, 1));
				}
				else
					m_CheckedBox = 0;
				updateBuffers();
			}
			return true;
		}
		return false;
		
	}
	void Dropdown::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		updateBuffers();
	}
	void Dropdown::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		updateBuffers();
	}
	void Dropdown::SetColor(const glm::vec3& color)
	{
		for (int i = 0; i < m_Box.size(); ++i)
		{
			m_Box[i].quad.SetColor(color);
		}
		updateBuffers();
	}
	void Dropdown::SetSize(const glm::vec2& size)
	{
		m_Size = size;
	}
	void Dropdown::StopRender()
	{
		m_Position = glm::vec2(-100.0f * (float)m_Box.size(),-100.0f * (float)m_Box.size());
		updateBuffers();
	}
	const glm::vec2& Dropdown::GetSize()
	{		
		return glm::vec2(m_Size.x , m_Size.y + m_Box[0].text.GetSize().y);
	}
	void Dropdown::AddBox(const std::string& text)
	{
	
		QuadVertex2D quad(m_Position,m_Size,glm::vec3(0,0,0),NULL);
		glm::vec2 position(m_Position.x,m_Position.y + ((m_Size.y + offset) * m_Box.size()));
		
		Dropbox box{quad,Text("Arial", text, glm::vec2(position.x + m_Size.x + 3, position.y + m_Size.y / 4.0f), glm::vec2(0.18f), glm::vec3(0.2, 1, 0.5), MAX_TEXT_CHAR),
		GuiApplication::Get()->PopIndex(GuiApplication::FRONT)};

		m_Box.push_back(box);
		updateBuffers();
	}
	const bool Dropdown::Clicked(int index)
	{
		if (m_CheckedBox == index)
		{
			m_Box[m_CheckedBox].quad.SetColor(glm::vec3(0,0,0));
			m_Minimized = !m_Minimized;
			minimize();
			m_CheckedBox = 0;
			return true;
		}
		return false;
	}
	const bool Dropdown::Checked(int index)
	{
		if (m_CheckedBox == index)
		{
			return true;
		}
		return false;
	}
	Ref<Widget> Dropdown::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		return std::make_shared<Dropdown>(label, position, size, color);
	}
	void Dropdown::updateBuffers()
	{
		for (int i = 1; i < m_Box.size(); ++i)
		{
			glm::vec2 newPos = m_Position;
			newPos.y += (m_Size.y + offset) * i;
			m_Box[i].quad.SetPosition(newPos, m_Size);
			m_Box[i].text.SetPosition(newPos);
			if (!m_Minimized)
			{
				GuiApplication::Get()->UpdateVertexBuffer(m_Box[i].index, &m_Box[i].quad, 1, GuiApplication::FRONT);
				GuiApplication::Get()->UpdateTextBuffer(m_Box[i].index, m_Box[i].text.GetVertice(0), m_Box[i].text.GetNumChar(), GuiApplication::FRONT);
			}
		}
		m_Box[0].quad.SetColor(glm::vec3(1, 1, 1));
		m_Box[0].quad.SetPosition(m_Position, m_Size);
		m_Box[m_CheckedBox].text.SetPosition(glm::vec2(m_Position.x + 5, m_Position.y + 2));
		GuiApplication::Get()->UpdateVertexBuffer(m_Box[0].index, &m_Box[0].quad, 1, GuiApplication::FRONT);
		GuiApplication::Get()->UpdateTextBuffer(m_Box[0].index, m_Box[m_CheckedBox].text.GetVertice(0),MAX_TEXT_CHAR, GuiApplication::FRONT);
	}
	void Dropdown::minimize()
	{
		if (m_Minimized)
		{
			for (int i = 1; i < m_Box.size(); ++i)
			{		
				m_Box[i].quad.SetPosition(glm::vec2(-100), glm::vec2(0));
				m_Box[i].text.SetPosition(glm::vec2(-100));
				GuiApplication::Get()->UpdateVertexBuffer(m_Box[i].index, &m_Box[i].quad, 1, GuiApplication::FRONT);
				GuiApplication::Get()->UpdateTextBuffer(m_Box[i].index, m_Box[i].text.GetVertice(0), m_Box[i].text.GetNumChar(), GuiApplication::FRONT);
			}
		}
		else
			updateBuffers();
	}
	glm::vec4 Dropdown::getCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}