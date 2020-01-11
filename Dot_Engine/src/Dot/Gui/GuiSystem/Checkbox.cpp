#include "stdafx.h"
#include "Checkbox.h"

#include "Dot/Utils/Text/Font.h"
namespace Dot {
	Checkbox::Checkbox( const glm::vec2& position, const glm::vec2& size,float labelsize)
		:
		m_Position(position),
		m_Size(size)
	{
	}
	
	bool Checkbox::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void Checkbox::UpdateData()
	{
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());

	}
	void Checkbox::ClickHandle()
	{
		m_Clicked = !m_Clicked;
		m_TexOffset = !m_TexOffset;
	
		glm::vec2 texcoords[4] = {
			   glm::vec2(0.25,0.25),
			   glm::vec2(float(m_TexOffset)/2,0.25),
			   glm::vec2(float(m_TexOffset)/2,0.5),
			   glm::vec2(0.25,0.5)
		};
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), &texcoords[0]);
		Gui::Get()->UpdateTextureBuffer(m_Index, &newVertex);	
	}

	void Checkbox::Exit()
	{
		
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);

		m_Label->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}

	void Checkbox::SetLabel(const Ref<Text> label)
	{
		m_Label = label;
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}

	const glm::vec2& Checkbox::GetLabelSize()
	{
		return m_Label->GetSize();
	}
	
	
	Checkbox& Checkbox::Get(const std::string& label)
	{	
		Checkbox& checkbox = (Checkbox&)Gui::Get()->GetWidget(label);
		return checkbox;
	}

	Checkbox& Checkbox::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		Checkbox& button = (Checkbox&)Gui::Get()->GetWrappedWidget(wrapper, label);
		return button;
	}

	void Checkbox::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize)
	{
		if (Gui::Get())
		{
			glm::vec2 texCoords[4] = {
					glm::vec2(0.25, 0.25),
					glm::vec2(0,   0.25),
					glm::vec2(0,   0.5),
					glm::vec2(0.25, 0.5)
			};

			QuadVertex quadVertex = QuadVertex(position, size, &texCoords[0]);
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Text> labelText = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
			Ref<Checkbox> checkbox = std::make_shared<Checkbox>(position, size);
			unsigned int index = Gui::Get()->AddWidget(label, checkbox, &quadVertex);

			checkbox->SetIndex(index);
			checkbox->SetLabel(labelText);
		}
	}

	glm::vec4 Checkbox::getCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}