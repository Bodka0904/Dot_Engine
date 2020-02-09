#include "stdafx.h"
#include "Slider.h"
#include "Dot/Gui/Gui/GuiApplication.h"

namespace Dot {
	Slider::Slider(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float* value, float rangeStart, float rangeEnd)
		:
		m_Label("Arial", label, glm::vec2(position.x, position.y-size.y + 10), glm::vec2(0.2f),glm::vec3(0.2, 1, 0.5), MAX_CHAR_PER_LABEL),
		m_Text("Arial", std::to_string(*value), glm::vec2(position.x + size.x + 2, position.y + 5), glm::vec2(0.18f), glm::vec3(0.2, 1, 0.5), MAX_TEXT_CHAR),
		m_Position(glm::vec2(position.x,position.y + 10)),
		m_Size(glm::vec2(size.x,10)),
		m_Value(value),
		m_Start(rangeStart),
		m_End(rangeEnd)
	{
		glm::vec2 texCoords[4] = {
				   glm::vec2(0.0f,		 0.0f),
				   glm::vec2(1.0f / 8.0f,0.0f),
				   glm::vec2(1.0f / 8.0f,1.0f / 8.0f),
				   glm::vec2(0.0f,		 1.0f / 8.0f)
		};
		glm::vec2 grabCoords[4] = {
				   glm::vec2(3.0f / 8.0f, 0.0f),
				   glm::vec2(4.0f / 8.0f, 0.0f),
				   glm::vec2(4.0f / 8.0f, 1.0f / 8.0f),
				   glm::vec2(3.0f / 8.0f, 1.0f / 8.0f)
		};	
		m_Quad = QuadVertex2D(m_Position, m_Size, color, &texCoords[0]);
		m_Index = GuiApplication::Get()->PopIndex();

		m_Grab.position = position;
		m_Grab.size = glm::vec2(size.y,size.y);
		m_Grab.quad = QuadVertex2D(position, glm::vec2(size.y, size.y), color, &grabCoords[0]);
		m_Grab.index = GuiApplication::Get()->PopIndex();
		updateBuffers();
	}

	Slider::~Slider()
	{
		Clean();
	}

	void Slider::Clean()
	{
		GuiApplication::Get()->PushIndex(m_Index);
		GuiApplication::Get()->PushIndex(m_Grab.index);

		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &QuadVertex2D());
		GuiApplication::Get()->UpdateVertexBuffer(m_Grab.index, &QuadVertex2D());
		std::vector<QuadVertex2D> quad;
		quad.resize(MAX_CHAR_PER_LABEL);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, &quad[0], MAX_CHAR_PER_LABEL);
		quad.resize(MAX_TEXT_CHAR);
		GuiApplication::Get()->UpdateTextBuffer(m_Index, &quad[0], MAX_TEXT_CHAR);
	}
	
	bool Slider::OnLeftClick(const glm::vec2& mousePos)
	{
		if (MouseHoover(mousePos))
		{
			m_Grab.grab = true;
			return true;
		}
		return false;
	}
	bool Slider::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{	
			return true;
		}
		m_Grab.grab = false;
		return false;
	}
	void Slider::Move(const glm::vec2& pos)
	{
		m_Grab.position += pos;
		updateBuffers();
	}
	void Slider::SetPosition(const glm::vec2& pos)
	{
		m_Grab.position = pos;	
		updateBuffers();
	}
	bool Slider::Release()
	{
		if (m_Grab.grab)
		{
			m_Grab.grab = false;
			return true;
		}
		return false;
	}

	void Slider::SetColor(const glm::vec3& color)
	{
		m_Quad.SetColor(color);
		updateBuffers();
	}

	void Slider::SetSize(const glm::vec2& size)
	{
		m_Size = size;
		m_Quad.SetPosition(m_Position, m_Size);
		m_Label.SetPosition(glm::vec2(m_Position.x + m_Size.x, m_Size.y));
		updateBuffers();
	}

	void Slider::StopRender()
	{
		m_Grab.position = glm::vec2(-100.0f,-100.0f) + m_Size;
		updateBuffers();
	}
	
	void Slider::Active(const glm::vec2& mousePos)
	{
		if (m_Grab.grab)
		{
			float offset = 0.3f;
			if ( mousePos.x-offset <= m_Position.x + m_Size.x && offset + mousePos.x >= m_Position.x)
			{
				float range = abs(m_Start) + m_End;
				float pos = (m_Grab.position.x - m_Position.x) / m_Size.x;
				*m_Value = m_Start + (pos * range);

				m_Text.Clear();
				m_Text.Push(std::to_string(*m_Value), glm::vec3(0.2, 1, 0.5));
				m_Grab.position.x = mousePos.x;

				m_Grab.quad.SetPosition(m_Grab.position, m_Grab.size);
				GuiApplication::Get()->UpdateVertexBuffer(m_Grab.index, &m_Grab.quad);
				GuiApplication::Get()->UpdateTextBuffer(m_Index, m_Text.GetVertice(0), MAX_TEXT_CHAR);
			}
		
		}
	}

	const glm::vec2& Slider::GetSize()
	{
		return glm::vec2(m_Size.x + m_Text.GetSize().x, m_Size.y + m_Label.GetSize().y);
	}
	Ref<Widget> Slider::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, float* value, float rangeStart, float rangeEnd)
	{
		Ref<Slider> slider = std::make_shared<Slider>(label, position, size, color, value, rangeStart, rangeEnd);
		return slider;
	}

	void Slider::updateBuffers()
	{
		m_Position = glm::vec2(m_Grab.position.x, m_Grab.position.y + 10);
		m_Grab.quad.SetPosition(m_Grab.position, m_Grab.size);
		m_Quad.SetPosition(m_Position, m_Size);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Grab.size.y));
		m_Text.SetPosition(glm::vec2(m_Grab.position.x + m_Size.x + 2, m_Grab.position.y + 5));

		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		GuiApplication::Get()->UpdateVertexBuffer(m_Grab.index, &m_Grab.quad);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, m_Label.GetVertice(0), m_Label.GetNumChar());
		GuiApplication::Get()->UpdateTextBuffer(m_Index, m_Text.GetVertice(0), MAX_TEXT_CHAR);
	}

	glm::vec4 Slider::getCoords()
	{
		return glm::vec4(m_Grab.position.x,
			m_Grab.position.y + m_Grab.size.y,
			m_Grab.position.x + m_Grab.size.x,
			m_Grab.position.y);
	}
}