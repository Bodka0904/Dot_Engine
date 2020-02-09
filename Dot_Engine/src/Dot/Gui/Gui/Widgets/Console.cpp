#include "stdafx.h"
#include "Console.h"
#include "Dot/Gui/Gui/GuiApplication.h"

#include "Dot/Core/KeyCodes.h"

namespace Dot {
	ConsoleText::ConsoleText(const glm::vec2& position, const glm::vec2& size)
		:
		Text("Arial", "", position, glm::vec2(0.18f), glm::vec3(1, 1, 1),MAX_CHAR_PER_CONSOLE),
		Size(size)
	{
		CharSize = float(Font::GetFont("Arial")->GetCharacter('O').sizeX) * 0.18f;
		Text.SetNumCharPerRow(unsigned int(Size.x / CharSize));
		LineHeight = float(Font::GetFont("Arial")->GetData().lineHeight) * 0.18f;
	}

	void ConsoleText::Push(std::string& text, Ref<Renderer2D>& renderer, const glm::vec3& color)
	{
		unsigned int oldNum = Text.GetNumChar();
		if (oldNum + text.size() >= MAX_CHAR_PER_CONSOLE)
		{
			Text.Clear();
			renderer->RestartBuffer();
			renderer->Push(Text.GetVertice(0), MAX_CHAR_PER_CONSOLE);
			renderer->PrepareForRender();
			oldNum = 0;
		}
		int counter = 0;
		for (char c : text)
		{			
			if (Size.y - LineHeight <= Text.GetSize().y)
			{
				Text.Clear();
				renderer->RestartBuffer();
				renderer->Push(Text.GetVertice(0), MAX_CHAR_PER_CONSOLE);
				renderer->PrepareForRender();
				
				oldNum = 0;
				// Push characters that were deleted;
				for (int i = 0; i < counter; ++i)
				{
					Text.PushCharacter(text.c_str()[i], color);		
				}
			}
			counter++;
			Text.PushCharacter(c, color);
		}
		renderer->PushOffset(Text.GetVertice(oldNum), (unsigned int)text.size(), oldNum);
	}

	void ConsoleText::SetSize(const glm::vec2& size)
	{ 
		Size = size; 
		Text.SetNumCharPerRow(unsigned int (Size.x/CharSize));
	}

	
	
	Console::Console(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label)
		:
		m_Label("Arial", label, glm::vec2(position.x, position.y + 4), glm::vec2(0.2f), labelColor, MAX_CHAR_PER_LABEL),
		m_Position(position),
		m_Size(size),
		m_Clicked(false)
	{
		m_Index = GuiApplication::Get()->PopIndex();
		m_CmdLine.Index = GuiApplication::Get()->PopIndex();

		glm::vec2 texCoords[4] = {
			glm::vec2(0.0f,		 0.0f),
			glm::vec2(1.0f / 8.0f,0.0f),
			glm::vec2(1.0f / 8.0f,1.0f / 8.0f),
			glm::vec2(0.0f,		 1.0f / 8.0f)
		};
		m_Quad = QuadVertex2D(position, size, glm::vec3(0, 0, 0), &texCoords[0]);
		m_CmdLine.Size = glm::vec2(m_Size.x, 20);
		m_CmdLine.Position = glm::vec2(position.x, position.y + size.y - m_CmdLine.Size.y);
		m_CmdLine.Quad = QuadVertex2D(m_CmdLine.Position, m_CmdLine.Size, glm::vec3(0, 0, 0), &texCoords[0]);

		glm::vec2 offset = glm::vec2(5, 2);
		m_CmdLine.Command = std::make_shared<Text>("Arial", "", m_CmdLine.Position + offset, glm::vec2(0.18f),glm::vec3(0.7f), MAX_TEXT_CHAR);
		m_TextRenderer = std::make_shared<Renderer2D>(MAX_CHAR_PER_CONSOLE);

		
		int maxRows = int(((double)m_Size.y - (double)m_CmdLine.Size.y - (double)m_Label.GetSize().y) / (Font::GetFont("Arial")->GetData().lineHeight * 0.2));
		
		m_Text = std::make_shared<ConsoleText>(position, glm::vec2(m_Size.x,m_Size.y - m_CmdLine.Size.y));

		updateBuffers();

		m_Command["clear"] = [&] {clearConsole(); };
	}
	Console::~Console()
	{
		Clean();
	}
	void Console::Clean()
	{
		GuiApplication::Get()->PushIndex(m_CmdLine.Index);
		GuiApplication::Get()->PushIndex(m_Index);
	}
	bool Console::MouseHoover(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			return true;
		}
		return false;
	}
	void Console::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		updateBuffers();
	}
	void Console::PushText(std::string& text, const glm::vec3& color)
	{
		m_Text->Push(text,m_TextRenderer,color);
	}
	bool Console::OnLeftClick(const glm::vec2& mousePos)
	{
		glm::vec4 coords = getCoords();

		if (mousePos.x >= coords.x && mousePos.x <= coords.z
			&& mousePos.y <= coords.y && mousePos.y >= coords.w)
		{
			if (mousePos.y >= coords.y - m_CmdLine.Size.y)
			{
				m_Clicked = !m_Clicked;
				m_CmdLine.Quad.SetColor(glm::vec3(m_Clicked, m_Clicked, m_Clicked));
				GuiApplication::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);
				return true;
			}
			return false;
		}
		return false;
	}

	void Console::SetX(float pos, float size)
	{
		m_Position.x = pos;
		m_Size.x = size;
		m_CmdLine.Position.x = pos;
		m_CmdLine.Size.x = size;
		m_Text->SetSize(m_Size);
		updateBuffers();
	}
	void Console::SetY(float pos, float height)
	{
		m_Position.y = pos;
		m_Size.y = height;
		m_CmdLine.Position.y = pos;
		m_Text->SetSize(m_Size);
		updateBuffers();
	}
	void Console::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;
		updateBuffers();
	}
	void Console::RegisterCommand(const std::string& cmd, std::function<void()> func)
	{
		if (cmd.size() > MAX_TEXT_CHAR)
			LOG_ERR("Max length of command is %s characters", MAX_TEXT_CHAR);
		if (m_Command.find(cmd) == m_Command.end())
			m_Command[cmd] = func;
		else
			LOG_ERR("Registering command already in existence");
	}
	
	void Console::SubmitCommand()
	{
		if (m_Clicked)
		{
			std::string cmd = m_CmdLine.Command->GetText();
			if (m_Command.find(cmd) != m_Command.end())
			{
				m_Command[cmd](); 
				PushText(cmd,glm::vec3(0.2,0.4,1));
			}
			else
				LOG_ERR("Command %s does not exist",cmd.c_str());
			
			m_CmdLine.Command->Clear();
			m_CmdLine.Command->RestartCurserX();
			m_CmdLine.Command->RestartCurserY();
			GuiApplication::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), m_CmdLine.Command->GetNumChar());
		}
	}
	bool Console::TakeInput(KeyPressedEvent& event)
	{
		if (m_Clicked)
		{
			switch (event.GetKey())
			{
				if (m_CmdLine.Command->GetNumChar() < MAX_TEXT_CHAR)
				{
			case D_KEY_ENTER:	SubmitCommand(); break;
			case D_KEY_SPACE:	m_CmdLine.Command->Push(" ", glm::vec3(0.7f)); break;
			case D_KEY_KP_0:	m_CmdLine.Command->Push("0", glm::vec3(0.7f)); break;
			case D_KEY_KP_1:	m_CmdLine.Command->Push("1", glm::vec3(0.7f)); break;
			case D_KEY_KP_2:	m_CmdLine.Command->Push("2", glm::vec3(0.7f)); break;
			case D_KEY_KP_3:	m_CmdLine.Command->Push("3", glm::vec3(0.7f)); break;
			case D_KEY_KP_4:	m_CmdLine.Command->Push("4", glm::vec3(0.7f)); break;
			case D_KEY_KP_5:	m_CmdLine.Command->Push("5", glm::vec3(0.7f)); break;
			case D_KEY_KP_6:	m_CmdLine.Command->Push("6", glm::vec3(0.7f)); break;
			case D_KEY_KP_7:	m_CmdLine.Command->Push("7", glm::vec3(0.7f)); break;
			case D_KEY_KP_8:	m_CmdLine.Command->Push("8", glm::vec3(0.7f)); break;
			case D_KEY_KP_9:	m_CmdLine.Command->Push("9", glm::vec3(0.7f)); break;
			case D_KEY_MINUS:	m_CmdLine.Command->Push("-", glm::vec3(0.7f)); break;
			case D_KEY_A:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("A", glm::vec3(0.7f)); else m_CmdLine.Command->Push("a", glm::vec3(0.7f)); break;
			case D_KEY_B:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("B", glm::vec3(0.7f)); else m_CmdLine.Command->Push("b", glm::vec3(0.7f)); break;
			case D_KEY_C:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("C", glm::vec3(0.7f)); else m_CmdLine.Command->Push("c", glm::vec3(0.7f)); break;
			case D_KEY_D:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("D", glm::vec3(0.7f)); else m_CmdLine.Command->Push("d", glm::vec3(0.7f)); break;
			case D_KEY_E:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("E", glm::vec3(0.7f)); else m_CmdLine.Command->Push("e", glm::vec3(0.7f)); break;
			case D_KEY_F:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("F", glm::vec3(0.7f)); else m_CmdLine.Command->Push("f", glm::vec3(0.7f)); break;
			case D_KEY_G:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("G", glm::vec3(0.7f)); else m_CmdLine.Command->Push("g", glm::vec3(0.7f)); break;
			case D_KEY_H:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("H", glm::vec3(0.7f)); else m_CmdLine.Command->Push("h", glm::vec3(0.7f)); break;
			case D_KEY_I:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("I", glm::vec3(0.7f)); else m_CmdLine.Command->Push("i", glm::vec3(0.7f)); break;
			case D_KEY_J:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("J", glm::vec3(0.7f)); else m_CmdLine.Command->Push("j", glm::vec3(0.7f)); break;
			case D_KEY_K:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("K", glm::vec3(0.7f)); else m_CmdLine.Command->Push("k", glm::vec3(0.7f)); break;
			case D_KEY_L:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("L", glm::vec3(0.7f)); else m_CmdLine.Command->Push("l", glm::vec3(0.7f)); break;
			case D_KEY_M:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("M", glm::vec3(0.7f)); else m_CmdLine.Command->Push("m", glm::vec3(0.7f)); break;
			case D_KEY_N:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("N", glm::vec3(0.7f)); else m_CmdLine.Command->Push("n", glm::vec3(0.7f)); break;
			case D_KEY_O:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("O", glm::vec3(0.7f)); else m_CmdLine.Command->Push("o", glm::vec3(0.7f)); break;
			case D_KEY_P:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("P", glm::vec3(0.7f)); else m_CmdLine.Command->Push("p", glm::vec3(0.7f)); break;
			case D_KEY_Q:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("Q", glm::vec3(0.7f)); else m_CmdLine.Command->Push("q", glm::vec3(0.7f)); break;
			case D_KEY_R:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("R", glm::vec3(0.7f)); else m_CmdLine.Command->Push("r", glm::vec3(0.7f)); break;
			case D_KEY_S:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("S", glm::vec3(0.7f)); else m_CmdLine.Command->Push("s", glm::vec3(0.7f)); break;
			case D_KEY_T:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("T", glm::vec3(0.7f)); else m_CmdLine.Command->Push("t", glm::vec3(0.7f)); break;
			case D_KEY_U:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("U", glm::vec3(0.7f)); else m_CmdLine.Command->Push("u", glm::vec3(0.7f)); break;
			case D_KEY_V:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("V", glm::vec3(0.7f)); else m_CmdLine.Command->Push("v", glm::vec3(0.7f)); break;
			case D_KEY_W:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("W", glm::vec3(0.7f)); else m_CmdLine.Command->Push("w", glm::vec3(0.7f)); break;
			case D_KEY_X:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("X", glm::vec3(0.7f)); else m_CmdLine.Command->Push("x", glm::vec3(0.7f)); break;
			case D_KEY_Y:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("Y", glm::vec3(0.7f)); else m_CmdLine.Command->Push("y", glm::vec3(0.7f)); break;
			case D_KEY_Z:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("Z", glm::vec3(0.7f)); else m_CmdLine.Command->Push("z", glm::vec3(0.7f)); break;
				}
			case D_KEY_BACKSPACE:	m_CmdLine.Command->Pop(); break;
			}

			GuiApplication::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), MAX_TEXT_CHAR);
			return true;
		}
		return false;
	}

	
	void Console::Render()
	{
		m_TextRenderer->Render();
	}
	void Console::updateBuffers()
	{
		m_Quad.SetPosition(m_Position, m_Size);
		m_CmdLine.Quad.SetPosition(m_CmdLine.Position, m_CmdLine.Size);
		m_Label.SetPosition(glm::vec2(m_Position.x, m_Position.y));
		
		m_Text->Text.SetPosition(glm::vec2(m_Position.x, m_Position.y + m_Label.GetSize().y));
		m_TextRenderer->RestartBuffer();
		m_TextRenderer->Push(m_Text->Text.GetVertice(0), m_Text->Text.GetNumChar());
		m_TextRenderer->PrepareForRender();

		
		glm::vec2 offset = glm::vec2(5, 2);
		m_CmdLine.Command->SetPosition(m_CmdLine.Position + offset);

		GuiApplication::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		GuiApplication::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);
		GuiApplication::Get()->UpdateLabelBuffer(m_Index, m_Label.GetVertice(0), m_Label.GetNumChar());
		GuiApplication::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), MAX_TEXT_CHAR);
	}

	void Console::clearConsole()
	{
		m_Text->Text.Clear();
		m_TextRenderer->RestartBuffer();
		m_TextRenderer->Push(m_Text->Text.GetVertice(0), MAX_CHAR_PER_CONSOLE);
		m_TextRenderer->PrepareForRender();
	}

	Ref<Console> Console::Create(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label)
	{
		Ref<Console> console = std::make_shared<Console>(position, size, labelColor, label);
		return console;
	}

	glm::vec4 Console::getCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}