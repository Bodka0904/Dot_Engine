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
		glm::vec2 newPos[4] =
		{
			glm::vec2(m_Position),
			glm::vec2(m_Position.x + m_Size.x,m_Position.y),
			glm::vec2(m_Position + m_Size),
			glm::vec2(m_Position.x,m_Position.y + m_Size.y)
		};
		Gui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label.GetSize().y));
	}
	void Button::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		glm::vec2 newPos[4] = 
		{
			glm::vec2(m_Position),
			glm::vec2(m_Position.x + m_Size.x,m_Position.y),
			glm::vec2(m_Position + m_Size),
			glm::vec2(m_Position.x,m_Position.y + m_Size.y)
		};
		Gui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);

		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label.GetSize().y));
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
		Button& button = (Button&)Gui::GetWidget(label);
		return button;
	}
	Button& Button::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		Button& button = (Button&)Gui::GetWrappeWidget(wrapper,label);
		return button;
	}
	void Button::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
			glm::vec2(0,0),
			glm::vec2(0.5,0),
			glm::vec2(0.5,0.25),
			glm::vec2(0,0.25)
		};
		Quad quad(position, size);
		Ref<Button> button = std::make_shared<Button>(label, position, size);
		Gui::AddWidget(label, button, quad, &texCoords[0]);
	}
	glm::vec4 Button::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}