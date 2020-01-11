#include "stdafx.h"
#include "TextArea.h"

#include "Dot/Utils/Text/Font.h"
#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/Input.h"

namespace Dot {
	TextArea::TextArea(const glm::vec2& position, const glm::vec2& size, TYPE type, float labelsize)
		:
		m_Position(position),
		m_Size(size),
		m_Type(type)
	{
		
	}
	bool TextArea::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	
	void TextArea::UpdateData()
	{
		QuadVertex newVertex = QuadVertex(m_Position, m_Size, NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		
		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y - m_Label->GetSize().y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
		
		glm::vec2 offset = glm::vec2(5, 2);
		m_Text->SetPosition(m_Position + offset);
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}
	void TextArea::ClickHandle()
	{
		m_Clicked = !m_Clicked;	
		m_TexOffset = !m_TexOffset;

		glm::vec2 texcoords[4] = {
				glm::vec2(0.75, 0.5),
				glm::vec2(0.5+ float(m_TexOffset) / 2, 0.5),
				glm::vec2(0.5+ float(m_TexOffset) / 2, 0.75),
				glm::vec2(0.75, 0.75)
		};
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), &texcoords[0]);
		Gui::Get()->UpdateTextureBuffer(m_Index, &newVertex);
	}
	void TextArea::Exit()
	{
		QuadVertex newVertex = QuadVertex(glm::vec2(0), glm::vec2(0), NULL);
		Gui::Get()->UpdatePosBuffer(m_Index, &newVertex);
		
		m_Label->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());

		m_Text->SetPosition(glm::vec2(-100, -100));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}
	void TextArea::SetLabel(const Ref<Text> label)
	{
		m_Label = label;
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
	}
	void TextArea::SetTextHandle(const Ref<Text> text)
	{
		m_Text = text;
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), m_Text->GetNumChar());
	}
	void TextArea::SetText(const std::string& text)
	{		
		m_Text->Clear();
		m_Text->Push(text);
		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), MAX_TEXT_CHAR);	
	}
	const glm::vec2& TextArea::GetLabelSize()
	{
		return m_Label->GetSize();
	}
	const void TextArea::TakeInput(KeyPressedEvent& event)
	{	
		int deletedChar = 0;
		switch (m_Type)
		{
		case Dot::TextArea::TYPE::INT:
			switch (event.GetKey())
			{
				if (m_Text->GetNumChar() < MAX_TEXT_CHAR)
				{
			case D_KEY_KP_0:		m_Text->Push("0"); break;
			case D_KEY_KP_1:		m_Text->Push("1"); break;
			case D_KEY_KP_2:		m_Text->Push("2"); break;
			case D_KEY_KP_3:		m_Text->Push("3"); break;
			case D_KEY_KP_4:		m_Text->Push("4"); break;
			case D_KEY_KP_5:		m_Text->Push("5"); break;
			case D_KEY_KP_6:		m_Text->Push("6"); break;
			case D_KEY_KP_7:		m_Text->Push("7"); break;
			case D_KEY_KP_8:		m_Text->Push("8"); break;
			case D_KEY_KP_9:		m_Text->Push("9"); break;
			case D_KEY_MINUS:		m_Text->Push("-"); break;
				}
			case D_KEY_BACKSPACE:	m_Text->Pop(); deletedChar++; break;
			}
			break;
		case Dot::TextArea::TYPE::FLOAT:
			switch (event.GetKey())
			{
				if (m_Text->GetNumChar() < MAX_TEXT_CHAR)
				{
			case D_KEY_KP_0:		m_Text->Push("0"); break;
			case D_KEY_KP_1:		m_Text->Push("1"); break;
			case D_KEY_KP_2:		m_Text->Push("2"); break;
			case D_KEY_KP_3:		m_Text->Push("3"); break;
			case D_KEY_KP_4:		m_Text->Push("4"); break;
			case D_KEY_KP_5:		m_Text->Push("5"); break;
			case D_KEY_KP_6:		m_Text->Push("6"); break;
			case D_KEY_KP_7:		m_Text->Push("7"); break;
			case D_KEY_KP_8:		m_Text->Push("8"); break;
			case D_KEY_KP_9:		m_Text->Push("9"); break;
			case D_KEY_PERIOD:		m_Text->Push("."); break;
			case D_KEY_MINUS:		m_Text->Push("-"); break;
				}
			case D_KEY_BACKSPACE:	m_Text->Pop(); deletedChar++; break;
			}
			break;
		case Dot::TextArea::TYPE::TEXT:
			switch (event.GetKey())
			{
				if (m_Text->GetNumChar() < MAX_TEXT_CHAR)
				{
			case D_KEY_KP_0:	m_Text->Push("0"); break;
			case D_KEY_KP_1:	m_Text->Push("1"); break;
			case D_KEY_KP_2:	m_Text->Push("2"); break;
			case D_KEY_KP_3:	m_Text->Push("3"); break;
			case D_KEY_KP_4:	m_Text->Push("4"); break;
			case D_KEY_KP_5:	m_Text->Push("5"); break;
			case D_KEY_KP_6:	m_Text->Push("6"); break;
			case D_KEY_KP_7:	m_Text->Push("7"); break;
			case D_KEY_KP_8:	m_Text->Push("8"); break;
			case D_KEY_KP_9:	m_Text->Push("9"); break;
			case D_KEY_MINUS:	m_Text->Push("-"); break;
			case D_KEY_A:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("A"); else m_Text->Push("a"); break;
			case D_KEY_B:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("B"); else m_Text->Push("b"); break;
			case D_KEY_C:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("C"); else m_Text->Push("c"); break;
			case D_KEY_D:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("D"); else m_Text->Push("d"); break;
			case D_KEY_E:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("E"); else m_Text->Push("e"); break;
			case D_KEY_F:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("F"); else m_Text->Push("f"); break;
			case D_KEY_G:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("G"); else m_Text->Push("g"); break;
			case D_KEY_H:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("H"); else m_Text->Push("h"); break;
			case D_KEY_I:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("I"); else m_Text->Push("i"); break;
			case D_KEY_J:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("J"); else m_Text->Push("j"); break;
			case D_KEY_K:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("K"); else m_Text->Push("k"); break;
			case D_KEY_L:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("L"); else m_Text->Push("l"); break;
			case D_KEY_M:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("M"); else m_Text->Push("m"); break;
			case D_KEY_N:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("N"); else m_Text->Push("n"); break;
			case D_KEY_O:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("O"); else m_Text->Push("o"); break;
			case D_KEY_P:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("P"); else m_Text->Push("p"); break;
			case D_KEY_Q:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("Q"); else m_Text->Push("q"); break;
			case D_KEY_R:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("R"); else m_Text->Push("r"); break;
			case D_KEY_S:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("S"); else m_Text->Push("s"); break;
			case D_KEY_T:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("T"); else m_Text->Push("t"); break;
			case D_KEY_U:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("U"); else m_Text->Push("u"); break;
			case D_KEY_V:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("V"); else m_Text->Push("v"); break;
			case D_KEY_W:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("W"); else m_Text->Push("w"); break;
			case D_KEY_X:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("X"); else m_Text->Push("x"); break;
			case D_KEY_Y:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("Y"); else m_Text->Push("y"); break;
			case D_KEY_Z:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text->Push("Z"); else m_Text->Push("z"); break;
				}
			case D_KEY_BACKSPACE:	m_Text->Pop(); deletedChar++; break;
			}
			break;
		}

		Gui::Get()->UpdateTextBuffer(m_Index, m_Text->GetVertice(0), MAX_TEXT_CHAR);
		
	}
	const bool& TextArea::GetClicked() const
	{
		if (Gui::Get())
		{
			return m_Clicked;
		}
		return false;
	}
	TextArea& TextArea::Get(const std::string& label)
	{
		
		TextArea& textarea = (TextArea&)Gui::Get()->GetWidget(label);
		return textarea;
	
	}
	TextArea& TextArea::GetWrapped(const std::string& wrapper, const std::string& label)
	{	
		TextArea& textarea = (TextArea&)Gui::Get()->GetWrappedWidget(wrapper, label);
		return textarea;
	}
	void TextArea::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, TYPE type, float labelsize)
	{
		if (Gui::Get())
		{
			glm::vec2 texCoords[4] = {
					glm::vec2(0.75, 0.5),
					glm::vec2(0.5, 0.5),
					glm::vec2(0.5, 0.75),
					glm::vec2(0.75, 0.75)
			};

			QuadVertex quadVertex = QuadVertex(position, size, &texCoords[0]);
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Text> labelText = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * labelsize), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
			Ref<TextArea> textarea = std::make_shared<TextArea>(position, size, type);
			unsigned int index = Gui::Get()->AddWidget(label, textarea, &quadVertex);

			glm::vec2 offset = glm::vec2(5, 2);
			Ref<Text> text = std::make_shared<Text>("Arial", "", position + offset, glm::vec2(labelsize, labelsize), MAX_TEXT_CHAR);
			switch (type)
			{
			case Dot::TextArea::TYPE::INT:
				text->Push("0");
				break;
			case Dot::TextArea::TYPE::FLOAT:
				text->Push("0");
				break;
			case Dot::TextArea::TYPE::TEXT:
				break;
			}

			textarea->SetIndex(index);
			textarea->SetLabel(labelText);
			textarea->SetTextHandle(text);
		}
	}
	glm::vec4 TextArea::getCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
	float TextArea::convertToFloat()
	{
		return strtof(m_Text->GetText().c_str(),0);
	}
	int TextArea::convertToInt()
	{
		std::stringstream ss(m_Text->GetText());
		int num = 0;
		ss >> num;
		return num;
	}
}