#include "stdafx.h"
#include "DArrowbutton.h"

namespace Dot {
	DArrbutton::DArrbutton(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		m_Position(position),
		m_Size(size)
	{
	}
	bool DArrbutton::MouseHoover(const glm::vec2& mousePos)
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
	void DArrbutton::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		glm::vec2 newPos[4] =
		{
			glm::vec2(pos),
			glm::vec2(pos.x + m_Size.x,pos.y),
			glm::vec2(pos + m_Size),
			glm::vec2(pos.x,pos.y + m_Size.y)
		};
		DGui::UpdatePosBuffer(m_Index, sizeof(glm::vec2) * 4, (void*)& newPos[0]);
	}
	DArrbutton& DArrbutton::Get(const std::string& label)
	{
		DArrbutton& button = (DArrbutton&)DGui::GetWidget(label);
		return button;
	}
	DArrbutton& DArrbutton::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		DArrbutton& arrbutton = (DArrbutton&)DGui::GetWrappedWidget(wrapper, label);
		return arrbutton;
	}
	void DArrbutton::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
				glm::vec2(0.5, 0),
				glm::vec2(1.0, 0),
				glm::vec2(1.0, 0.25),
				glm::vec2(0.5, 0.25)
		};

		DQuad quad(position, size);
		Ref<DArrbutton> button = std::make_shared<DArrbutton>(label, position, size);
		DGui::AddWidget(label, button, quad, &texCoords[0]);
	}
	glm::vec4 DArrbutton::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}