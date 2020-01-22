#include "stdafx.h"
#include "TextArea.h"
#include "Dot/Gui/Gui/GuiApplication.h"

#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Core/Input.h"

namespace Dot {
	TextArea::TextArea(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, const glm::vec3& textColor)
		:
		m_Label("Arial", label, glm::vec2(position.x, position.y - Font::GetFont("Arial")->GetData().lineHeight * 0.2f), glm::vec2(0.2f), glm::vec3(0.2, 1, 0.5), MAX_CHAR_PER_LABEL),
		m_Text("Arial", "", glm::vec2(position.x + 5, position.y+2), glm::vec2(0.18f), textColor, MAX_TEXT_CHAR),
		m_Position(position),
		m_TextColor(textColor),
		m_Size(size)
	{
		glm::vec2 texCoords[4] = {
				   glm::vec2(4.0f / 8.0f, 0.0f),
				   glm::vec2(5.0f / 8.0f, 0.0f),
				   glm::vec2(5.0f / 8.0f, 1.0f / 8.0f),
				   glm::vec2(4.0f / 8.0f, 1.0f / 8.0f)
		};
		m_Quad = QuadVertex2D(position, size, color, &texCoords[0]);
		m_Index = GuiApplication::Get()->PopIndex();
		updateBuffers();
	}
	TextArea::~TextArea()
	{
		GuiApplication::Get()->PushIndex(m_Index);
		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &QuadVertex2D());

		std::vector<QuadVertex2D> quad;
		quad.resize(MAX_CHAR_PER_LABEL);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, &quad[0], MAX_CHAR_PER_LABEL);
		quad.resize(MAX_TEXT_CHAR);
		GuiApplication::Get()->UpdateTextBuffer(m_Index, &quad[0], MAX_TEXT_CHAR);
	}
	bool TextArea::OnLeftClick(const glm::vec2& mousePos)
	{
		if (MouseHoover(mousePos))
		{
			m_Clicked = !m_Clicked;
			m_Quad.SetColor(glm::vec3(1.0f - (float(m_Clicked) / 4)));
			GuiApplication::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
			
			return true;
		}
		return false;
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
	void TextArea::Move(const glm::vec2& pos)
	{
		m_Position += pos;		
		updateBuffers();
	}
	void TextArea::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		updateBuffers();
	}
	bool TextArea::Release()
	{
		return false;
	}

	void TextArea::SetColor(const glm::vec3& color)
	{
		m_Quad.SetColor(color);
		updateBuffers();
	}

	void TextArea::SetSize(const glm::vec2& size)
	{
		m_Size = size;
		m_Quad.SetPosition(m_Position, m_Size);
		m_Label.SetPosition(glm::vec2(m_Position.x + m_Size.x, m_Size.y));
		updateBuffers();
	}
	void TextArea::StopRender()
	{
		m_Position = glm::vec2(-100);
		updateBuffers();
	}
	void TextArea::TakeInput(KeyPressedEvent& event, TYPE type)
	{
		if (m_Clicked)
		{
			if (m_Text.GetSize().x < m_Size.x)
			{
				int deletedChar = 0;
				switch (type)
				{
				case Dot::TextArea::TYPE::INT:
					switch (event.GetKey())
					{
						if (m_Text.GetNumChar() < MAX_TEXT_CHAR)
						{
					case D_KEY_KP_0:		m_Text.Push("0", m_TextColor); break;
					case D_KEY_KP_1:		m_Text.Push("1", m_TextColor); break;
					case D_KEY_KP_2:		m_Text.Push("2", m_TextColor); break;
					case D_KEY_KP_3:		m_Text.Push("3", m_TextColor); break;
					case D_KEY_KP_4:		m_Text.Push("4", m_TextColor); break;
					case D_KEY_KP_5:		m_Text.Push("5", m_TextColor); break;
					case D_KEY_KP_6:		m_Text.Push("6", m_TextColor); break;
					case D_KEY_KP_7:		m_Text.Push("7", m_TextColor); break;
					case D_KEY_KP_8:		m_Text.Push("8", m_TextColor); break;
					case D_KEY_KP_9:		m_Text.Push("9", m_TextColor); break;
					case D_KEY_MINUS:		m_Text.Push("-", m_TextColor); break;
						}
					case D_KEY_BACKSPACE:	m_Text.Pop(); deletedChar++; break;
					}
					break;
				case Dot::TextArea::TYPE::FLOAT:
					switch (event.GetKey())
					{
						if (m_Text.GetNumChar() < MAX_TEXT_CHAR)
						{
					case D_KEY_KP_0:		m_Text.Push("0", m_TextColor); break;
					case D_KEY_KP_1:		m_Text.Push("1", m_TextColor); break;
					case D_KEY_KP_2:		m_Text.Push("2", m_TextColor); break;
					case D_KEY_KP_3:		m_Text.Push("3", m_TextColor); break;
					case D_KEY_KP_4:		m_Text.Push("4", m_TextColor); break;
					case D_KEY_KP_5:		m_Text.Push("5", m_TextColor); break;
					case D_KEY_KP_6:		m_Text.Push("6", m_TextColor); break;
					case D_KEY_KP_7:		m_Text.Push("7", m_TextColor); break;
					case D_KEY_KP_8:		m_Text.Push("8", m_TextColor); break;
					case D_KEY_KP_9:		m_Text.Push("9", m_TextColor); break;
					case D_KEY_PERIOD:		m_Text.Push(".", m_TextColor); break;
					case D_KEY_MINUS:		m_Text.Push("-", m_TextColor); break;
						}
					case D_KEY_BACKSPACE:	m_Text.Pop(); deletedChar++; break;
					}
					break;
				case Dot::TextArea::TYPE::TEXT:
					switch (event.GetKey())
					{
						if (m_Text.GetNumChar() < MAX_TEXT_CHAR)
						{
					case D_KEY_KP_0:	m_Text.Push("0", m_TextColor); break;
					case D_KEY_KP_1:	m_Text.Push("1", m_TextColor); break;
					case D_KEY_KP_2:	m_Text.Push("2", m_TextColor); break;
					case D_KEY_KP_3:	m_Text.Push("3", m_TextColor); break;
					case D_KEY_KP_4:	m_Text.Push("4", m_TextColor); break;
					case D_KEY_KP_5:	m_Text.Push("5", m_TextColor); break;
					case D_KEY_KP_6:	m_Text.Push("6", m_TextColor); break;
					case D_KEY_KP_7:	m_Text.Push("7", m_TextColor); break;
					case D_KEY_KP_8:	m_Text.Push("8", m_TextColor); break;
					case D_KEY_KP_9:	m_Text.Push("9", m_TextColor); break;
					case D_KEY_MINUS:	m_Text.Push("-", m_TextColor); break;
					case D_KEY_A:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("A", m_TextColor); else m_Text.Push("a", m_TextColor); break;
					case D_KEY_B:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("B", m_TextColor); else m_Text.Push("b", m_TextColor); break;
					case D_KEY_C:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("C", m_TextColor); else m_Text.Push("c", m_TextColor); break;
					case D_KEY_D:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("D", m_TextColor); else m_Text.Push("d", m_TextColor); break;
					case D_KEY_E:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("E", m_TextColor); else m_Text.Push("e", m_TextColor); break;
					case D_KEY_F:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("F", m_TextColor); else m_Text.Push("f", m_TextColor); break;
					case D_KEY_G:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("G", m_TextColor); else m_Text.Push("g", m_TextColor); break;
					case D_KEY_H:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("H", m_TextColor); else m_Text.Push("h", m_TextColor); break;
					case D_KEY_I:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("I", m_TextColor); else m_Text.Push("i", m_TextColor); break;
					case D_KEY_J:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("J", m_TextColor); else m_Text.Push("j", m_TextColor); break;
					case D_KEY_K:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("K", m_TextColor); else m_Text.Push("k", m_TextColor); break;
					case D_KEY_L:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("L", m_TextColor); else m_Text.Push("l", m_TextColor); break;
					case D_KEY_M:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("M", m_TextColor); else m_Text.Push("m", m_TextColor); break;
					case D_KEY_N:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("N", m_TextColor); else m_Text.Push("n", m_TextColor); break;
					case D_KEY_O:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("O", m_TextColor); else m_Text.Push("o", m_TextColor); break;
					case D_KEY_P:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("P", m_TextColor); else m_Text.Push("p", m_TextColor); break;
					case D_KEY_Q:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("Q", m_TextColor); else m_Text.Push("q", m_TextColor); break;
					case D_KEY_R:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("R", m_TextColor); else m_Text.Push("r", m_TextColor); break;
					case D_KEY_S:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("S", m_TextColor); else m_Text.Push("s", m_TextColor); break;
					case D_KEY_T:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("T", m_TextColor); else m_Text.Push("t", m_TextColor); break;
					case D_KEY_U:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("U", m_TextColor); else m_Text.Push("u", m_TextColor); break;
					case D_KEY_V:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("V", m_TextColor); else m_Text.Push("v", m_TextColor); break;
					case D_KEY_W:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("W", m_TextColor); else m_Text.Push("w", m_TextColor); break;
					case D_KEY_X:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("X", m_TextColor); else m_Text.Push("x", m_TextColor); break;
					case D_KEY_Y:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("Y", m_TextColor); else m_Text.Push("y", m_TextColor); break;
					case D_KEY_Z:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_Text.Push("Z", m_TextColor); else m_Text.Push("z", m_TextColor); break;
						}
					case D_KEY_BACKSPACE:	m_Text.Pop(); deletedChar++; break;
					}
					break;
				}
			}
			else
			{
				if (event.GetKey() == D_KEY_BACKSPACE)
					m_Text.Pop();
			}
			event.IsHandled() = true;
			GuiApplication::Get()->UpdateTextBuffer(m_Index, m_Text.GetVertice(0), MAX_TEXT_CHAR);
		}
	}

	const glm::vec2& TextArea::GetSize()
	{
		return glm::vec2(m_Size.x + m_Text.GetSize().x, m_Size.y + m_Label.GetSize().y);
	}
	
	Ref<Widget> TextArea::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, const glm::vec3& color,const glm::vec3& textColor)
	{
		Ref<TextArea> textarea = std::make_shared<TextArea>(label, position, size, color,textColor);
		return textarea;
	}

	void TextArea::updateBuffers()
	{
		m_Quad.SetPosition(m_Position, m_Size);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y - Font::GetFont("Arial")->GetData().lineHeight * 0.2f));
		m_Text.SetPosition(glm::vec2(m_Position.x + 5, m_Position.y + 2));
		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, m_Label.GetVertice(0), m_Label.GetNumChar());
		GuiApplication::Get()->UpdateTextBuffer(m_Index, m_Text.GetVertice(0), MAX_TEXT_CHAR);
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
		return strtof(m_Text.GetText().c_str(), 0);
	}
	int TextArea::convertToInt()
	{
		std::stringstream ss(m_Text.GetText());
		int num = 0;
		ss >> num;
		return num;
	}
}