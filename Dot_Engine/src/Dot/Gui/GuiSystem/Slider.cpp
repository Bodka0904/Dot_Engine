#include "stdafx.h"
#include "Slider.h"

#include "Dot/Utils/Text/Font.h"
namespace Dot {
	Slider::Slider(const std::string& label, const glm::vec2& position, const glm::vec2& size,float * value,float labelsize)
		:
		m_Position(position),
		m_Size(size),
		m_Value(value),
		m_Label("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize))
	{
	}
	bool Slider::MouseHoover(const glm::vec2& mousePos)
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
	void Slider::Move(const glm::vec2 pos)
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
	void Slider::SetPosition(const glm::vec2& pos)
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
	void Slider::ClickHandle()
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
			Gui::UpdateTexBuffer(m_Index, sizeof(Quad), &texcoords[0]);
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
			Gui::UpdateTexBuffer(m_Index, sizeof(Quad), &texcoords[0]);
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
			Gui::UpdateTexBuffer(m_Index, sizeof(Quad), &texcoords[0]);
		}
	}
	const glm::vec2& Slider::GetLabelSize()
	{
		return m_Label.GetSize();
	}
	Slider& Slider::Get(const std::string& label)
	{
		Slider& slider = (Slider&)Gui::GetWidget(label);
		return slider;
	}
	Slider& Slider::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		Slider& slider = (Slider&)Gui::GetWrappeWidget(wrapper, label);
		return slider;
	}
	void Slider::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float *val)
	{
		glm::vec2 texCoords[4] = {
				glm::vec2(0.5, 0.5),
				glm::vec2(1, 0.5),
				glm::vec2(1, 0.75),
				glm::vec2(0.5, 0.75)
		};

		Quad quad(position, size);
		Ref<Slider> slider = std::make_shared<Slider>(label, position, size, val);
		Gui::AddWidget(label, slider, quad, &texCoords[0]);
	}
	glm::vec4 Slider::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}