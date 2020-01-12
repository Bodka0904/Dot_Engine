#include "stdafx.h"
#include "Arrowbutton.h"

#include "Dot/Utils/Text/Font.h"
namespace Dot {

	Arrbutton::Arrbutton(const std::string& label,const glm::vec2& position, const glm::vec2& size,float labelsize)
		:
		m_Position(position),
		m_Size(size)
	{
		m_LeftClick = false;
		m_RightClick = false;
		m_Clicked = &m_LeftClick;

		glm::vec2 texCoords[4] = {
				glm::vec2(0.25, 0),
				glm::vec2(0.5, 0),
				glm::vec2(0.5, 0.25),
				glm::vec2(0.25, 0.25)
		};

		m_Quad = QuadVertex(position, size, &texCoords[0]);

		m_Index = Gui::Get()->PopIndex();
		m_Label = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);

		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
	}
	bool Arrbutton::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();
		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			if (mousePos.x < coords.z - m_Size.x / 2)
			{
				m_Clicked = &m_LeftClick;
			}
			else
			{
				m_Clicked = &m_RightClick;
			}
			return true;
		}
		return false;
	}
	
	
	void Arrbutton::Exit()
	{
		m_Quad.SetPosition(glm::vec2(0), glm::vec2(0));
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		
		m_Label->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}

	void Arrbutton::Move(const glm::vec2 pos)
	{
		m_Position += pos;
		m_Quad.Move(pos);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}

	void Arrbutton::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		m_Quad.Move(pos);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	
	const glm::vec2& Arrbutton::GetLabelSize()
	{
		return m_Label->GetSize();
	}
	const bool Arrbutton::LeftClicked()
	{
		if (m_LeftClick == true)
		{
			m_LeftClick = false;
			return true;
		}
		return false;
	}
	const bool Arrbutton::RightClicked()
	{
		if (m_RightClick == true)
		{
			m_RightClick = false;
			return true;
		}
		return false;
	}
	Arrbutton& Arrbutton::Get(const std::string& label)
	{
		Arrbutton arrbutton = (Arrbutton&)Gui::Get()->GetWidget(label);
		return arrbutton;
	}
	Arrbutton& Arrbutton::GetWrapped(const std::string& wrapper, const std::string& label)
	{		
		return (Arrbutton&)Gui::Get()->GetWrappedWidget(wrapper, label);;	
	}
	void Arrbutton::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize)
	{
		if (Gui::Get())
		{
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Arrbutton> arrbutton = std::make_shared<Arrbutton>(label,position, size,labelsize);
			Gui::Get()->AddWidget(label, arrbutton);
		}
	}
	glm::vec4 Arrbutton::getCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}