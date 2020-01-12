#include "stdafx.h"
#include "Slider.h"

#include "Dot/Utils/Text/Font.h"
namespace Dot {
	Slider::Slider(const std::string& label,const glm::vec2& position, const glm::vec2& size,float * value, float range,float labelsize)
		:
		m_Position(position),
		m_Size(size),
		m_Value(value),
		m_Range(range)
	{
		glm::vec2 texCoords[4] = {
					   glm::vec2(0.25f, 0.5f),
					   glm::vec2(0.5f, 0.5f),
					   glm::vec2(0.5f, 0.75f),
					   glm::vec2(0.25f, 0.75f)
		};
		m_Quad = QuadVertex(position, size, &texCoords[0]);
		m_Index = Gui::Get()->PopIndex();
		m_Label = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
		glm::vec2 offset = glm::vec2(5, 0);
		m_Text= std::make_shared<Text>("Arial", "0", m_Position + offset, glm::vec2(labelsize, labelsize), MAX_TEXT_CHAR);
	
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
	}
	bool Slider::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();
		
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
	
	void Slider::ClickHandle()
	{
		*m_Value = m_TempStorage * m_Range;
		if (m_TempStorage >= 0.98f)
		{
			m_TexOffset = -m_TempStorage - 0.1f;	
			m_Quad.vertices[0].texCoord = glm::vec2(0.25f + m_TexOffset / 4, 0.5f);
			m_Quad.vertices[1].texCoord = glm::vec2(0.5f + m_TexOffset / 4, 0.5f);
			m_Quad.vertices[2].texCoord = glm::vec2(0.5f + m_TexOffset / 4, 0.75f);
			m_Quad.vertices[3].texCoord = glm::vec2(0.25f + m_TexOffset / 4, 0.75f);
			
			Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		}
		else if (m_TempStorage <= 0.02f)
		{
			m_TexOffset = -m_TempStorage;

			m_Quad.vertices[0].texCoord = glm::vec2(0.25f + m_TexOffset / 4, 0.5f);
			m_Quad.vertices[1].texCoord = glm::vec2(0.5f + m_TexOffset / 4, 0.5f);
			m_Quad.vertices[2].texCoord = glm::vec2(0.5f + m_TexOffset / 4, 0.75f);
			m_Quad.vertices[3].texCoord = glm::vec2(0.25f + m_TexOffset / 4, 0.75f);
			
			Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		}
		else
		{
			m_TexOffset = -m_TempStorage;
			
			m_Quad.vertices[0].texCoord = glm::vec2(0.25f + m_TexOffset / 4, 0.5f);
			m_Quad.vertices[1].texCoord = glm::vec2(0.5f + m_TexOffset / 4, 0.5f);
			m_Quad.vertices[2].texCoord = glm::vec2(0.5f + m_TexOffset / 4, 0.75f);
			m_Quad.vertices[3].texCoord = glm::vec2(0.25f + m_TexOffset / 4, 0.75f);
			
			Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		}

		m_Text->SetPositionInBuffer(0);
		m_Text->RestartCurserX();

		m_Text->Push(std::to_string(*m_Value));
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}
	void Slider::Exit()
	{	
		m_Quad.SetPosition(glm::vec2(0), glm::vec2(0));
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

		m_Label->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());

		m_Text->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}

	void Slider::Move(const glm::vec2 pos)
	{
		m_Position += pos;
		m_Quad.Move(pos);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());

		glm::vec2 offset = glm::vec2(5, 2);
		m_Text->SetPosition(m_Position + offset);
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}

	void Slider::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		m_Quad.SetPosition(pos,m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());

		glm::vec2 offset = glm::vec2(5, 2);
		m_Text->SetPosition(m_Position + offset);
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}
	
	
	const glm::vec2& Slider::GetLabelSize()
	{
		return m_Label->GetSize();
	}
	Slider& Slider::Get(const std::string& label)
	{	
		Slider& slider = (Slider&)Gui::Get()->GetWidget(label);
		return slider;
	}
	Slider& Slider::GetWrapped(const std::string& wrapper, const std::string& label)
	{	
		Slider& slider = (Slider&)Gui::Get()->GetWrappedWidget(wrapper, label);
		return slider;
	}
	void Slider::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float *val,float range,float labelsize)
	{
		if (Gui::Get())
		{
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Slider> slider = std::make_shared<Slider>(label,position, size, val, range,labelsize);
			Gui::Get()->AddWidget(label, slider);
		}
	}
	glm::vec4 Slider::getCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}