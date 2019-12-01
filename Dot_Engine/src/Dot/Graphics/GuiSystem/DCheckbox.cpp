#include "stdafx.h"
#include "DCheckbox.h"

namespace Dot {
	DCheckbox::DCheckbox(const std::string& label, const glm::vec2& position, const glm::vec2& size)
		:
		m_Position(position),
		m_Size(size)
	{
	}
	
	bool DCheckbox::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void DCheckbox::SetPosition(const glm::vec2& pos)
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
	void DCheckbox::ClickHandle()
	{
		m_Clicked = !m_Clicked;
		m_TexOffset = !m_TexOffset;
		glm::vec2 texcoords[4] = {
			   glm::vec2(0.5,0.25),
			   glm::vec2(m_TexOffset,0.25),
			   glm::vec2(m_TexOffset,0.5),
			   glm::vec2(0.5,0.5)
		};
		DGui::UpdateTexBuffer(m_Index, sizeof(DQuad), &texcoords[0]);
	}
	
	DCheckbox& DCheckbox::Get(const std::string& label)
	{
		DCheckbox& checkbox = (DCheckbox&)DGui::GetWidget(label);
		return checkbox;
	}

	DCheckbox& DCheckbox::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		DCheckbox& button = (DCheckbox&)DGui::GetWrappedWidget(wrapper, label);
		return button;
	}

	void DCheckbox::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
				glm::vec2(0.5, 0.25),
				glm::vec2(0,   0.25),
				glm::vec2(0,   0.5),
				glm::vec2(0.5, 0.5)
		};
		 
		DQuad quad(position, size);
		Ref<DCheckbox> checkbox = std::make_shared<DCheckbox>(label, position, size);
		DGui::AddWidget(label, checkbox, quad, &texCoords[0]);
	}

	glm::vec4 DCheckbox::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}