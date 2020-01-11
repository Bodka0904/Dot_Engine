#include "stdafx.h"
#include "Button.h"

#include "Dot/Utils/Text/Font.h"

namespace Dot {
	Button::Button(const glm::vec2& position, const glm::vec2& size, float labelsize)
		: 
		m_Position(position),
		m_Size(size)
	{
	}

	bool Button::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void Button::UpdateData()
	{
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);

		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	void Button::Exit()
	{
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		
		m_Label->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	void Button::SetLabel(const Ref<Text> label)
	{
		m_Label = label;
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	const glm::vec2& Button::GetLabelSize()
	{
		return m_Label->GetSize();
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
		Button& button = (Button&)Gui::Get()->GetWrappedWidget(wrapper, label);
		return button;	
	}
	void Button::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize)
	{
		if (Gui::Get())
		{
			glm::vec2 texCoords[4] = {
				glm::vec2(0,0),
				glm::vec2(0.25,0),
				glm::vec2(0.25,0.25),
				glm::vec2(0,0.25)
			};
			QuadVertex quadVertex = QuadVertex(position, size, &texCoords[0]);
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Text> labelText = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
			Ref<Button> button = std::make_shared<Button>(position, size);
			unsigned int index = Gui::Get()->AddWidget(label, button, &quadVertex);

			button->SetIndex(index);
			button->SetLabel(labelText);
		}
	}
	glm::vec4 Button::getCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}