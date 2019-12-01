#include "stdafx.h"
#include "DSlider.h"

namespace Dot {
	DSlider::DSlider(const std::string& label, const glm::vec2& position, const glm::vec2& size,float * value)
		:
		m_Position(position),
		m_Size(size),
		m_Value(value)
	{
	}
	bool DSlider::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();
		
		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			m_TempStorage = ((mousePos.x - m_Position.x) / m_Size.x);
			if (m_TempStorage <= 0.02f)
			{
				m_TempStorage = 0.0f;
			}
			else if (m_TempStorage >= 0.96f)
			{
				m_TempStorage = 1.0f;
			}
		
			return true;
		}
		
		return false;
	}
	void DSlider::SetPosition(const glm::vec2& pos)
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
	void DSlider::ClickHandle()
	{
		*m_Value = m_TempStorage;
		if (*m_Value >= 0.96f)
		{
			m_TexOffset = -*m_Value - 0.1;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.5 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.75),
					glm::vec2(0.5 + m_TexOffset / 2,0.75)
			};
			DGui::UpdateTexBuffer(m_Index, sizeof(DQuad), &texcoords[0]);
		}
		else if (*m_Value <= 0.02f)
		{
			m_TexOffset = -*m_Value + 0.1;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.5 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.75),
					glm::vec2(0.5 + m_TexOffset / 2,0.75)
			};
			DGui::UpdateTexBuffer(m_Index, sizeof(DQuad), &texcoords[0]);
		}
		else
		{
			m_TexOffset = -*m_Value;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.5 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.5),
					glm::vec2(1 + m_TexOffset / 2,0.75),
					glm::vec2(0.5 + m_TexOffset / 2,0.75)
			};
			DGui::UpdateTexBuffer(m_Index, sizeof(DQuad), &texcoords[0]);
		}
	}
	DSlider& DSlider::Get(const std::string& label)
	{
		DSlider& slider = (DSlider&)DGui::GetWidget(label);
		return slider;
	}
	DSlider& DSlider::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		DSlider& slider = (DSlider&)DGui::GetWrappedWidget(wrapper, label);
		return slider;
	}
	void DSlider::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float *val)
	{
		glm::vec2 texCoords[4] = {
				glm::vec2(0.5, 0.5),
				glm::vec2(1, 0.5),
				glm::vec2(1, 0.75),
				glm::vec2(0.5, 0.75)
		};

		DQuad quad(position, size);
		Ref<DSlider> slider = std::make_shared<DSlider>(label, position, size, val);
		DGui::AddWidget(label, slider, quad, &texCoords[0]);
	}
	glm::vec4 DSlider::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}