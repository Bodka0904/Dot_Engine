#include "stdafx.h"
#include "Slider.h"

#include "Dot/Utils/Text/Font.h"
namespace Dot {
	Slider::Slider(const glm::vec2& position, const glm::vec2& size,float * value, float range,float labelsize)
		:
		m_Position(position),
		m_Size(size),
		m_Value(value),
		m_Range(range)
	{
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
	void Slider::UpdateData()
	{
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());

		glm::vec2 offset = glm::vec2(5, 0);
		m_Text->SetPosition(glm::vec2(m_Position.x + m_Size.x + offset.x, m_Position.y));
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}
	void Slider::ClickHandle()
	{
		*m_Value = m_TempStorage * m_Range;
		if (m_TempStorage >= 0.96f)
		{
			m_TexOffset = -m_TempStorage - 0.1f;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.25f + m_TexOffset / 4,0.5f),
					glm::vec2(0.5f + m_TexOffset  / 4,0.5f),
					glm::vec2(0.5f + m_TexOffset  / 4,0.75f),
					glm::vec2(0.25f + m_TexOffset / 4,0.75f)
			};
			QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), &texcoords[0]);
			Gui::Get()->UpdateTextureBuffer(m_Index, &newVertex);
		}
		else if (m_TempStorage <= 0.02f)
		{
			m_TexOffset = -m_TempStorage;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.25f + m_TexOffset / 4,0.5f),
					glm::vec2(0.5f + m_TexOffset / 4,0.5f),
					glm::vec2(0.5f + m_TexOffset / 4,0.75f),
					glm::vec2(0.25f + m_TexOffset / 4,0.75f)
			};
			QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), &texcoords[0]);
			Gui::Get()->UpdateTextureBuffer(m_Index, &newVertex);
		}
		else
		{
			m_TexOffset = -m_TempStorage;
			glm::vec2 texcoords[4] = {
					glm::vec2(0.25f + m_TexOffset / 4,0.5f),
					glm::vec2(0.5f + m_TexOffset / 4,0.5f),
					glm::vec2(0.5f + m_TexOffset / 4,0.75f),
					glm::vec2(0.25f + m_TexOffset / 4,0.75f)
			};
			QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), &texcoords[0]);
			Gui::Get()->UpdateTextureBuffer(m_Index, &newVertex);
		}

		m_Text->SetPositionInBuffer(0);
		m_Text->RestartCurserX();

		m_Text->Push(std::to_string(*m_Value));
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}
	void Slider::Exit()
	{	
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);

		m_Label->SetPosition(glm::vec2(-100.0f, -100.0f));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
		
		m_Text->SetPosition(glm::vec2(-100.0f, -100.0f));
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}
	void Slider::SetLabel(const Ref<Text> label)
	{
		m_Label = label;
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	void Slider::SetTextHandle(const Ref<Text> text)
	{
		m_Text = text;
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
			glm::vec2 texCoords[4] = {
					glm::vec2(0.25f, 0.5f),
					glm::vec2(0.5f, 0.5f),
					glm::vec2(0.5f, 0.75f),
					glm::vec2(0.25f, 0.75f)
			};

			QuadVertex quadVertex = QuadVertex(position, size, &texCoords[0]);
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Text> labelText = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
			Ref<Slider> slider = std::make_shared<Slider>(position, size, val, range);
			unsigned int index = Gui::Get()->AddWidget(label, slider, &quadVertex);


			glm::vec2 offset = glm::vec2(5, 0);
			Ref<Text> text = std::make_shared<Text>("Arial", "0", glm::vec2(position.x + size.x + offset.x, position.y), glm::vec2(labelsize, labelsize), MAX_TEXT_CHAR);

			slider->SetIndex(index);
			slider->SetLabel(labelText);
			slider->SetTextHandle(text);
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