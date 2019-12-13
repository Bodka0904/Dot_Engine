#include "stdafx.h"
#include "Arrowbutton.h"

#include "Dot/Utils/Text/Font.h"
namespace Dot {

	Arrbutton::Arrbutton(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize)
		:
		m_Position(position),
		m_Size(size),
		m_Label("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize))
	{
		m_LeftClick = false;
		m_RightClick = false;
		m_Clicked = &m_LeftClick;
	}
	bool Arrbutton::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();
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
	void Arrbutton::Move(const glm::vec2 pos)
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
	void Arrbutton::SetPosition(const glm::vec2& pos)
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
	void Arrbutton::Minimize()
	{
		glm::vec2 newPos[4] =
		{
			glm::vec2(0),
			glm::vec2(0),
			glm::vec2(0),
			glm::vec2(0)
		};
		Gui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
		m_Label.SetPosition(glm::vec2(-100, -100));
	}
	const glm::vec2& Arrbutton::GetLabelSize()
	{
		return m_Label.GetSize();
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
		Arrbutton arrbutton = (Arrbutton&)Gui::GetWidget(label);
		return arrbutton;
	}
	Arrbutton& Arrbutton::GetWrapped(const std::string& wrapper, const std::string& label)
	{	
		return (Arrbutton&)Gui::GetWrappeWidget(wrapper, label);;
	}
	void Arrbutton::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
				glm::vec2(0.25, 0),
				glm::vec2(0.5, 0),
				glm::vec2(0.5, 0.25),
				glm::vec2(0.25, 0.25)
		};

		Quad quad(position, size);
		Ref<Arrbutton> arrbutton = std::make_shared<Arrbutton>(label, position, size);
		Gui::AddWidget(label, arrbutton, quad, &texCoords[0]);
	}
	glm::vec4 Arrbutton::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}