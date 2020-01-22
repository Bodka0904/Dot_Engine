#include "stdafx.h"
#include "ArrowButton.h"
#include "Dot/Gui/Gui/GuiApplication.h"

namespace Dot {
	Dot::ArrowButton::ArrowButton(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
		:
		m_Label("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * 0.2f), glm::vec2(0.2f), glm::vec3(0.2, 1, 0.5), MAX_CHAR_PER_LABEL),
		m_Text("Arial", label, glm::vec2(position.x + size.x + 3, position.y + size.y / 4.0f), glm::vec2(0.18f), glm::vec3(0.2, 1, 0.5), MAX_TEXT_CHAR),
		m_Position(position),
		m_Size(size)
	{
		glm::vec2 leftTexCoords[4] = {
				   glm::vec2(5.0f / 8.0f,0.0f),
				   glm::vec2(6.0f / 8.0f,0.0f),
				   glm::vec2(6.0f / 8.0f,1.0f / 8.0f),
				   glm::vec2(5.0f / 8.0f,1.0f / 8.0f)
		};
		glm::vec2 rightTexCoords[4] = {
				   glm::vec2(6.0f / 8.0f,0.0f),
				   glm::vec2(7.0f / 8.0f,0.0f),
				   glm::vec2(7.0f / 8.0f,1.0f / 8.0f),
				   glm::vec2(6.0f / 8.0f,1.0f / 8.0f)
		};

		m_Size = size;
		m_Position = position;
		m_Left.index = GuiApplication::Get()->PopIndex();
		m_Left.size = glm::vec2(size.x / 2.0f, size.y);
		m_Left.quad = QuadVertex2D(position, glm::vec2(size.x / 2.0f, size.y), glm::vec3(1, 1, 1), &leftTexCoords[0]);

		m_Right.index = GuiApplication::Get()->PopIndex();
		m_Right.size = glm::vec2(size.x / 2.0f, size.y);
		m_Right.quad = QuadVertex2D(glm::vec2(position.x + size.x / 2.0f, position.y), glm::vec2(size.x / 2.0f, size.y), glm::vec3(1, 1, 1), &rightTexCoords[0]);
		updateBuffers();
	}

	Dot::ArrowButton::~ArrowButton()
	{
		GuiApplication::Get()->PushIndex(m_Left.index);
		GuiApplication::Get()->UpdateVertexBuffer(m_Left.index, &QuadVertex2D());

		GuiApplication::Get()->PushIndex(m_Right.index);
		GuiApplication::Get()->UpdateVertexBuffer(m_Right.index, &QuadVertex2D());

		std::vector<QuadVertex2D> quad;
		quad.resize(MAX_CHAR_PER_LABEL);
		GuiApplication::Get()->UpdateLabelBuffer(m_Left.index, &quad[0], MAX_CHAR_PER_LABEL);
		quad.resize(MAX_TEXT_CHAR);
		GuiApplication::Get()->UpdateTextBuffer(m_Left.index, &quad[0], MAX_TEXT_CHAR);
	}

	bool Dot::ArrowButton::OnLeftClick(const glm::vec2& mousePos)
	{
		if (MouseHoover(mousePos))
		{
			glm::vec4 coords = getCoords();
			if (mousePos.x < coords.z - m_Left.size.x)
			{
				m_Left.clicked = true;
				m_Left.quad.SetColor(glm::vec3(0.3, 0.3, 1));
				GuiApplication::Get()->UpdateVertexBuffer(m_Left.index, &m_Left.quad);
			}
			else
			{
				m_Right.clicked = true;
				m_Right.quad.SetColor(glm::vec3(0.3, 0.3, 1));
				GuiApplication::Get()->UpdateVertexBuffer(m_Right.index, &m_Right.quad);
			}
			return true;
		}
		return false;
	}

	bool Dot::ArrowButton::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{		
			return true;
		}
		return false;
	}

	bool Dot::ArrowButton::Release()
	{
		m_Left.quad.SetColor(glm::vec3(1, 1, 1));
		GuiApplication::Get()->UpdateVertexBuffer(m_Left.index, &m_Left.quad);

		m_Right.quad.SetColor(glm::vec3(1, 1, 1));
		GuiApplication::Get()->UpdateVertexBuffer(m_Right.index, &m_Right.quad);
		return false;
	}

	void Dot::ArrowButton::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		updateBuffers();
	}

	void Dot::ArrowButton::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		updateBuffers();
	}

	void Dot::ArrowButton::SetColor(const glm::vec3& color)
	{
		m_Left.quad.SetColor(color);
		m_Right.quad.SetColor(color);
		updateBuffers();
	}

	void Dot::ArrowButton::SetSize(const glm::vec2& size)
	{
		
	}

	void Dot::ArrowButton::StopRender()
	{
		m_Position = glm::vec2(-100);
		updateBuffers();
	}

	const glm::vec2& Dot::ArrowButton::GetSize()
	{
		return glm::vec2(m_Size.x + m_Text.GetSize().x, m_Size.y + m_Label.GetSize().y);
	}

	const bool ArrowButton::LeftClicked()
	{
		if (m_Left.clicked)
		{
			m_Left.clicked = false;
			return true;
		}
		return false;
	}

	const bool ArrowButton::RightClicked()
	{
		if (m_Right.clicked)
		{
			m_Right.clicked = false;
			return true;
		}
		return false;
	}


	Ref<Widget> Dot::ArrowButton::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		Ref<ArrowButton> button = std::make_shared<ArrowButton>(label, position, size, color);
		return button;
	}



	void Dot::ArrowButton::updateBuffers()
	{
		m_Left.quad.SetPosition(m_Position, m_Left.size);
		m_Right.quad.SetPosition(glm::vec2(m_Position.x + m_Left.size.x, m_Position.y),m_Right.size);
		m_Text.SetPosition(glm::vec2(m_Position.x + m_Size.x + 3, m_Position.y + m_Size.y / 4.0f));
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - Font::GetFont("Arial")->GetData().lineHeight * 0.2f));
	
		GuiApplication::Get()->UpdateVertexBuffer(m_Left.index, &m_Left.quad);
		GuiApplication::Get()->UpdateVertexBuffer(m_Right.index, &m_Right.quad);
		GuiApplication::Get()->UpdateLabelBuffer(m_Left.index, m_Label.GetVertice(0), m_Label.GetNumChar());
		GuiApplication::Get()->UpdateTextBuffer(m_Left.index, m_Text.GetVertice(0), m_Text.GetNumChar());
	}

	glm::vec4 Dot::ArrowButton::getCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}