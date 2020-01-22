#include "stdafx.h"
#include "CheckBox.h"
#include "Dot/Gui/Gui/GuiApplication.h"

namespace Dot {
	CheckBox::CheckBox(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
		:
		m_Label("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * 0.2f), glm::vec2(0.2f), glm::vec3(0.2, 1, 0.5), MAX_CHAR_PER_LABEL),
		m_Text("Arial", label, glm::vec2(position.x + size.x + 3, position.y + size.y / 4.0f), glm::vec2(0.18f), glm::vec3(0.2, 1, 0.5), MAX_TEXT_CHAR),
		m_Position(position),
		m_Size(size)
	{
		glm::vec2 texCoords[4] = {
				   glm::vec2(1.0f / 8.0f, 0.0f),
				   glm::vec2(2.0f / 8.0f, 0.0f),
				   glm::vec2(2.0f / 8.0f, 1.0f / 8.0f),
				   glm::vec2(1.0f / 8.0f, 1.0f / 8.0f)
		};

		m_Quad = QuadVertex2D(position, size, color, &texCoords[0]);
		m_Index = GuiApplication::Get()->PopIndex();
		updateBuffers();
	}
	CheckBox::~CheckBox()
	{
		GuiApplication::Get()->PushIndex(m_Index);
		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &QuadVertex2D());

		std::vector<QuadVertex2D> quad;
		quad.resize(MAX_CHAR_PER_LABEL);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, &quad[0], MAX_CHAR_PER_LABEL);
		quad.resize(MAX_TEXT_CHAR);
		GuiApplication::Get()->UpdateTextBuffer(m_Index, &quad[0], MAX_TEXT_CHAR);
	}
	bool CheckBox::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{			
			return true;
		}
		return false;
	}
	bool CheckBox::OnLeftClick(const glm::vec2& mousePos)
	{
		if (MouseHoover(mousePos))
		{
			m_Clicked = !m_Clicked;
			glm::vec2 texCoords[4] = {
				   glm::vec2((1.0f + m_Clicked) / 8.0f, 0.0f),
				   glm::vec2((2.0f + m_Clicked) / 8.0f, 0.0f),
				   glm::vec2((2.0f + m_Clicked) / 8.0f, 1.0f / 8.0f),
				   glm::vec2((1.0f + m_Clicked) / 8.0f, 1.0f / 8.0f)
			};
			m_Quad.vertices[0].texCoord = texCoords[0];
			m_Quad.vertices[1].texCoord = texCoords[1];
			m_Quad.vertices[2].texCoord = texCoords[2];
			m_Quad.vertices[3].texCoord = texCoords[3];

			updateBuffers();
			
			return true;
		}
		return false;
	}
	void CheckBox::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		updateBuffers();
	}
	void CheckBox::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;	
		updateBuffers();
	}
	void CheckBox::SetColor(const glm::vec3& color)
	{
		m_Quad.SetColor(color);
		updateBuffers();
	}
	void CheckBox::SetSize(const glm::vec2& size)
	{
		m_Size = size;
		m_Quad.SetPosition(m_Position, m_Size);
		m_Label.SetPosition(glm::vec2(m_Position.x + m_Size.x, m_Size.y));
		updateBuffers();
	}
	void CheckBox::StopRender()
	{
		m_Position = glm::vec2(-100);
		updateBuffers();
	}
	const glm::vec2& CheckBox::GetSize()
	{
		return glm::vec2(m_Size.x + m_Text.GetSize().x, m_Size.y + m_Label.GetSize().y);
	}
	const bool CheckBox::Clicked()
	{
		return m_Clicked;
	}
	Ref<Widget> CheckBox::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color)
	{
		Ref<CheckBox> button = std::make_shared<CheckBox>(label, position, size, color);
		return button;
	}
	void CheckBox::updateBuffers()
	{
		m_Quad.SetPosition(m_Position, m_Size);
		m_Text.SetPosition(glm::vec2(m_Position.x + m_Size.x + 3, m_Position.y + m_Size.y / 4.0f));
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - Font::GetFont("Arial")->GetData().lineHeight * 0.2f));
		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, m_Label.GetVertice(0), m_Label.GetNumChar());
		GuiApplication::Get()->UpdateTextBuffer(m_Index, m_Text.GetVertice(0), m_Text.GetNumChar());
	}
	glm::vec4 CheckBox::getCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}