#include "stdafx.h"
#include "Button.h"

#include "Dot/Utils/Text/Font.h"

namespace Dot {
	Button::Button(const std::string& label,const glm::vec2& position, const glm::vec2& size, float labelsize)
		: 
		m_Position(position),
		m_Size(size)
	{
		glm::vec2 texCoords[4] = {
				   glm::vec2(0,0),
				   glm::vec2(0.25,0),
				   glm::vec2(0.25,0.25),
				   glm::vec2(0,0.25)
		};
		m_Quad = QuadVertex(position, size, &texCoords[0]);
		m_Index = Gui::Get()->PopIndex();
		m_Label = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
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
	
	void Button::Exit()
	{
		m_Quad.SetPosition(glm::vec2(0), glm::vec2(0));
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		
		m_Label->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}

	void Button::Move(const glm::vec2 pos)
	{
		m_Position += pos;
		m_Quad.Move(pos);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}

	void Button::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		m_Quad.SetPosition(pos,m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
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
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);		
			Ref<Button> button = std::make_shared<Button>(label,position, size,labelsize);
			Gui::Get()->AddWidget(label, button);
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