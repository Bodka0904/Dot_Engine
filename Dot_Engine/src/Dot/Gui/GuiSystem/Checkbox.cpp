#include "stdafx.h"
#include "Checkbox.h"

#include "Dot/Utils/Text/Font.h"
namespace Dot {
	Checkbox::Checkbox(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize)
		:
		m_Position(position),
		m_Size(size)
	{
		glm::vec2 texCoords[4] = {
					glm::vec2(0.25, 0.25),
					glm::vec2(0,   0.25),
					glm::vec2(0,   0.5),
					glm::vec2(0.25, 0.5)
		};
	
		m_Quad = QuadVertex(position, size, &texCoords[0]);
		m_Index = Gui::Get()->PopIndex();
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
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
	
	void Checkbox::ClickHandle()
	{
		m_Clicked = !m_Clicked;
		m_TexOffset = !m_TexOffset;
	
		m_Quad.vertices[0].texCoord = glm::vec2(0.25, 0.25);
		m_Quad.vertices[1].texCoord = glm::vec2(float(m_TexOffset) / 2, 0.25);
		m_Quad.vertices[2].texCoord = glm::vec2(float(m_TexOffset) / 2, 0.5);
		m_Quad.vertices[3].texCoord = glm::vec2(0.25, 0.5);

		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
	}

	void Checkbox::Exit()
	{	
		m_Quad.SetPosition(glm::vec2(0), glm::vec2(0));
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

		m_Label->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}

	void Checkbox::Move(const glm::vec2 pos)
	{
		m_Position += pos;
		m_Quad.Move(pos);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}

	void Checkbox::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		m_Quad.Move(pos);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
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
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Checkbox> checkbox = std::make_shared<Checkbox>(label, position, size,labelsize);
			Gui::Get()->AddWidget(label,checkbox);
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