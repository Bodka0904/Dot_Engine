#include "stdafx.h"
#include "DButton.h"


namespace Dot {
	DButton::DButton(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		: 
		m_Position(position),
		m_Size(size)
	{
	}

	bool DButton::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void DButton::SetPosition(const glm::vec2& pos)
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
	const bool DButton::GetClicked()
	{
		if (m_Clicked == true)
		{
			m_Clicked = false;
			return true;
		}
		return false;
	}
	DButton& DButton::Get(const std::string& label)
	{
		DButton& button = (DButton&)DGui::GetWidget(label);
		return button;
	}
	DButton& DButton::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		DButton& button = (DButton&)DGui::GetWrappedWidget(wrapper,label);
		return button;
	}
	void DButton::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
			glm::vec2(0,0),
			glm::vec2(0.5,0),
			glm::vec2(0.5,0.25),
			glm::vec2(0,0.25)
		};
		DQuad quad(position, size);
		Ref<DButton> button = std::make_shared<DButton>(label, position, size);
		DGui::AddWidget(label, button, quad, &texCoords[0]);
	}
	glm::vec4 DButton::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}