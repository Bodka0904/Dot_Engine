#include "stdafx.h"
#include "Checkbox.h"

#include "Dot/Utils/Text/Font.h"
namespace Dot {
	Checkbox::Checkbox(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize)
		:
		m_Position(position),
		m_Size(size),
		m_Label("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize)) {
	}
	
	bool Checkbox::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = GetCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void Checkbox::Move(const glm::vec2 pos)
	{
		m_Position += pos;
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label.GetSize().y));
	}
	void Checkbox::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;

		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label.GetSize().y));

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
		Gui::Get()->UpdateTexBuffer(m_Index, &newVertex);	
	}

	void Checkbox::Minimize()
	{
		
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		m_Label.SetPosition(glm::vec2(-100, -100));
	}

	const glm::vec2& Checkbox::GetLabelSize()
	{
		return m_Label.GetSize();
	}
	
	Checkbox& Checkbox::Get(const std::string& label)
	{
		Checkbox& checkbox = (Checkbox&)Gui::Get()->GetWidget(label);
		return checkbox;
	}

	Checkbox& Checkbox::GetWrapped(const std::string& wrapper, const std::string& label)
	{
		Checkbox& button = (Checkbox&)Gui::Get()->GetWrappeWidget(wrapper, label);
		return button;
	}

	void Checkbox::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 texCoords[4] = {
				glm::vec2(0.25, 0.25),
				glm::vec2(0,   0.25),
				glm::vec2(0,   0.5),
				glm::vec2(0.25, 0.5)
		};
		 
		QuadVertex quadVertex = QuadVertex(position,size,&texCoords[0]);
		Ref<Checkbox> checkbox = std::make_shared<Checkbox>(label, position, size);
		Gui::Get()->AddWidget(label, checkbox,&quadVertex);
	}

	glm::vec4 Checkbox::GetCoords()
	{
		return glm::vec4(m_Position.x,
						 m_Position.y + m_Size.y,
						 m_Position.x + m_Size.x,
						 m_Position.y);
	}
}