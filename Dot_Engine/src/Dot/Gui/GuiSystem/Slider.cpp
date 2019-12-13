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
		;
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label.GetSize().y));
	}
	void Slider::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label.GetSize().y));
	}
	void Slider::ClickHandle()
	{
		*m_Value = m_TempStorage;
		if (*m_Value >= 0.96f)
		{
			m_TexOffset = -*m_Value - 0.1;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.25 + m_TexOffset / 4,0.5),
					glm::vec2(0.5 + m_TexOffset / 4,0.5),
					glm::vec2(0.5 + m_TexOffset / 4,0.75),
					glm::vec2(0.25 + m_TexOffset / 4,0.75)
			};
			QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), &texcoords[0]);
			Gui::Get()->UpdateTexBuffer(m_Index, &newVertex);
		}
		else if (*m_Value <= 0.02f)
		{
			m_TexOffset = -*m_Value;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.25 + m_TexOffset / 4,0.5),
					glm::vec2(0.5 + m_TexOffset / 4,0.5),
					glm::vec2(0.5 + m_TexOffset / 4,0.75),
					glm::vec2(0.25 + m_TexOffset / 4,0.75)
			};
			QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), &texcoords[0]);
			Gui::Get()->UpdateTexBuffer(m_Index, &newVertex);
		}
		else
		{
			m_TexOffset = -*m_Value;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.25 + m_TexOffset / 4,0.5),
					glm::vec2(0.5 + m_TexOffset / 4,0.5),
					glm::vec2(0.5 + m_TexOffset / 4,0.75),
					glm::vec2(0.25 + m_TexOffset / 4,0.75)
			};
			QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), &texcoords[0]);
			Gui::Get()->UpdateTexBuffer(m_Index, &newVertex);
		}
	}
	void Slider::Minimize()
	{
		
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		m_Label.SetPosition(glm::vec2(-100, -100));
	}
	const glm::vec2& Slider::GetLabelSize()
	{
		return m_Label.GetSize();
	}
	Slider& Slider::Get(const std::string& label)
	{
		Slider& slider = (Slider&)Gui::Get()->GetWidget(label);
		return slider;
	}
	Slider& Slider::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		Slider& slider = (Slider&)Gui::Get()->GetWrappeWidget(wrapper, label);
		return slider;
	}
	void Slider::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float *val)
	{
		glm::vec2 texCoords[4] = {
				glm::vec2(0.25, 0.5),
				glm::vec2(0.5, 0.5),
				glm::vec2(0.5, 0.75),
				glm::vec2(0.25, 0.75)
		};
		QuadVertex quadVertex = QuadVertex(position, size,&texCoords[0]);
		Ref<Slider> slider = std::make_shared<Slider>(label, position, size, val);
		Gui::Get()->AddWidget(label, slider,&quadVertex);
	}
	glm::vec4 Slider::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}