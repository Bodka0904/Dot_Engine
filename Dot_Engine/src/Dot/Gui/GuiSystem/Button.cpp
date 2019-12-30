#include "stdafx.h"
#include "Button.h"

#include "Dot/Utils/Text/Font.h"

namespace Dot {
	Button::Button(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize)
		: 
		m_Position(position),
		m_Size(size),
		m_Label("Arial",label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize))
	{
		
	}

	bool Button::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void Button::Move(const glm::vec2 pos)
	{
		m_Position += pos;
		
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label.GetSize().y));
	}
	void Button::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);

		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label.GetSize().y));
	}
	void Button::Minimize()
	{
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		m_Label.SetPosition(glm::vec2(-100, -100));
	}
	const glm::vec2& Button::GetLabelSize()
	{
		return m_Label.GetSize();
	}
	const bool Button::GetClicked()
	{
		if (m_Clicked == true)
		{
			m_Clicked = false;
			return true;
		}
		return false;
	}
	Button& Button::Get(const std::string& label)
	{
		Button& button = (Button&)Gui::Get()->GetWidget(label);
		return button;
	}
	Button& Button::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		Button& button = (Button&)Gui::Get()->GetWrappedWidget(wrapper,label);
		return button;
	}
	void Button::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
			glm::vec2(0,0),
			glm::vec2(0.25,0),
			glm::vec2(0.25,0.25),
			glm::vec2(0,0.25)
		};
		QuadVertex quadVertex = QuadVertex(position, size, &texCoords[0]);
		Ref<Button> button = std::make_shared<Button>(label, position, size);
		Gui::Get()->AddWidget(label, button, &quadVertex);
	}
	glm::vec4 Button::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}