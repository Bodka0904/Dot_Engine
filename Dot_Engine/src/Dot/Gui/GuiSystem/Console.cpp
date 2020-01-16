#include "stdafx.h"
#include "Console.h"

#include "Gui.h"

#include "Dot/Core/KeyCodes.h"

namespace Dot {
	Console::Console(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize, float textSize)
		:
		m_Position(position),
		m_Size(size),
		m_Index(0),
		m_TextColor(1, 1, 1),
		m_Clicked(false)
	{
		glm::vec2 texCoords[4] = {
			glm::vec2(0, 0.75),
			glm::vec2(0.25, 0.75),
			glm::vec2(0.25, 1),
			glm::vec2(0, 1),
		};
		m_Quad = QuadVertex(position, size, &texCoords[0]);
		
		m_CmdLine.Size = glm::vec2(m_Size.x, 20);
		m_CmdLine.Position = glm::vec2(position.x, position.y + size.y - m_CmdLine.Size.y);
		glm::vec2 texCoordsCmd[4] = {
			glm::vec2(0.0f, 0.5f),
			glm::vec2(0.25f, 0.5f),
			glm::vec2(0.25f, 0.75f),
			glm::vec2(0.0f, 0.75f)
		};

		m_CmdLine.Quad = QuadVertex(m_CmdLine.Position,m_CmdLine.Size,&texCoordsCmd[0]);

		m_Index = Gui::Get()->PopIndex();
		m_CmdLine.Index = Gui::Get()->PopIndex();

		glm::vec2 offset = glm::vec2(5, 2);
		m_CmdLine.Command = std::make_shared<Text>("Arial", "", m_CmdLine.Position + offset, glm::vec2(labelsize, labelsize), MAX_TEXT_CHAR);

		m_Label = std::make_shared<Text>("Arial", label, glm::vec2(position.x, position.y), glm::vec2(labelsize, labelsize), MAX_CHAR_PER_LABEL);
		m_Text = std::make_shared<Text>("Arial", "", glm::vec2(position.x, position.y + m_Label->GetSize().y), glm::vec2(textSize, textSize), MAX_CHAR_PER_CONSOLE, m_Size.x - 30);
		m_TextRenderer = std::make_shared<Renderer2D>(MAX_CHAR_PER_CONSOLE);

		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);
		Gui::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());
		
		m_Command["clear"] = [&] {Clear(); };
	}

	Console::~Console()
	{
		Gui::Get()->PushIndex(m_Index);
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

	void Console::RegisterCommand(const std::string& cmd, std::function<void()> func)
	{
		if (cmd.size() > MAX_TEXT_CHAR)
			LOG_ERR("Max length of command is %s characters", MAX_TEXT_CHAR);
		if (m_Command.find(cmd) == m_Command.end())
			m_Command[cmd] = func;
		else
			LOG_ERR("Registering command already in existence");
	}

	void Console::Render(const Ref<Shader>& textShader, const Ref<OrthoCamera> camera)
	{
		m_TextRenderer->BeginScene(textShader, camera);
		{
			Font::Bind("Arial");
			textShader->UploadUniformFloat3("u_Color", m_TextColor);
			m_TextRenderer->Render();
		}
		m_TextRenderer->EndScene();
	}

	void Console::Move(const glm::vec2& pos)
	{
		m_Position += pos;
		m_Quad.Move(pos);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

		m_CmdLine.Quad.Move(pos);
		m_CmdLine.Position += pos;
		Gui::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);

		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());

		glm::vec2 offset = glm::vec2(5, 2);
		m_CmdLine.Command->SetPosition(m_CmdLine.Position + offset);
		Gui::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), m_CmdLine.Command->GetNumChar());

		m_Text->SetPosition(glm::vec2(m_Position.x, m_Position.y + m_Label->GetSize().y));
		m_TextRenderer->RestartBuffer();
		m_TextRenderer->Push(m_Text->GetVertice(0), m_Text->GetNumChar());
		m_TextRenderer->PrepareForRender();
	}

	void Console::SetPosition(const glm::vec2& pos)
	{
		m_Position = pos;

		m_Quad.SetPosition(m_Position,m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

		m_CmdLine.Position = glm::vec2(m_Position.x, m_Position.y + m_Size.y - m_CmdLine.Size.y);
		m_CmdLine.Quad.SetPosition(m_CmdLine.Position,m_CmdLine.Size);
		Gui::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);

		m_Label->SetPosition(glm::vec2(m_Position.x, m_Position.y));
		Gui::Get()->UpdateLabelBuffer(m_Index, m_Label->GetVertice(0), m_Label->GetNumChar());

		glm::vec2 offset = glm::vec2(5, 2);
		m_CmdLine.Command->SetPosition(m_CmdLine.Position + offset);
		Gui::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), m_CmdLine.Command->GetNumChar());

		m_Text->SetPosition(glm::vec2(m_Position.x, m_Position.y + m_Label->GetSize().y));
		m_TextRenderer->RestartBuffer();
		m_TextRenderer->Push(m_Text->GetVertice(0), m_Text->GetNumChar());
		m_TextRenderer->PrepareForRender();
	}

	void Console::Clear()
	{
		m_Text->Clear();
		m_Text->RestartCurserX();
		m_Text->RestartCurserY();
		m_TextRenderer->RestartBuffer();
		m_TextRenderer->Push(m_Text->GetVertice(0), m_Text->GetNumChar());
		m_TextRenderer->PrepareForRender();
		m_Size.y = 200;
		m_Quad.SetPosition(m_Position, m_Size);
		Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

		m_CmdLine.Position = glm::vec2(m_Position.x, m_Position.y + m_Size.y - m_CmdLine.Size.y);
		m_CmdLine.Quad.SetPosition(m_CmdLine.Position, m_CmdLine.Size);
		Gui::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);

		glm::vec2 offset = glm::vec2(5, 2);
		m_CmdLine.Command->SetPosition(m_CmdLine.Position + offset);
		Gui::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), m_CmdLine.Command->GetNumChar());
		LOG_WARN("Clearing console");
	}

	void Console::PushText(const std::string& text)
	{
		unsigned int oldNum = m_Text->GetNumChar();
		if (oldNum + text.size() >= MAX_CHAR_PER_CONSOLE)
		{
			Clear();
			LOG_WARN("Conole reached maximum of characters, clear console");
		}
		else
		{
			m_Text->Push(text);
			m_TextRenderer->PushOffset(m_Text->GetVertice(oldNum), text.size(), oldNum);

			if (m_Text->GetSize().y >= m_Size.y - m_CmdLine.Size.y)
			{
				m_Size.y = m_Text->GetSize().y + m_CmdLine.Size.y;
				m_Quad.SetPosition(m_Position, m_Size);
				Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

				m_CmdLine.Position = glm::vec2(m_Position.x, m_Position.y + m_Size.y - m_CmdLine.Size.y);
				m_CmdLine.Quad.SetPosition(m_CmdLine.Position, m_CmdLine.Size);
				Gui::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);

				glm::vec2 offset = glm::vec2(5, 2);
				m_CmdLine.Command->SetPosition(m_CmdLine.Position + offset);
				Gui::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), m_CmdLine.Command->GetNumChar());
			}
		}
	}

	void Console::TakeInput(KeyPressedEvent& event)
	{	
		switch (event.GetKey())
		{
			if (m_Text->GetNumChar() < MAX_TEXT_CHAR)
			{
				case D_KEY_ENTER:	SubmitCommand(); break;
				case D_KEY_SPACE:	m_CmdLine.Command->Push(" "); break;
				case D_KEY_KP_0:	m_CmdLine.Command->Push("0"); break;
				case D_KEY_KP_1:	m_CmdLine.Command->Push("1"); break;
				case D_KEY_KP_2:	m_CmdLine.Command->Push("2"); break;
				case D_KEY_KP_3:	m_CmdLine.Command->Push("3"); break;
				case D_KEY_KP_4:	m_CmdLine.Command->Push("4"); break;
				case D_KEY_KP_5:	m_CmdLine.Command->Push("5"); break;
				case D_KEY_KP_6:	m_CmdLine.Command->Push("6"); break;
				case D_KEY_KP_7:	m_CmdLine.Command->Push("7"); break;
				case D_KEY_KP_8:	m_CmdLine.Command->Push("8"); break;
				case D_KEY_KP_9:	m_CmdLine.Command->Push("9"); break;
				case D_KEY_MINUS:	m_CmdLine.Command->Push("-"); break;
				case D_KEY_A:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("A"); else m_CmdLine.Command->Push("a"); break;
				case D_KEY_B:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("B"); else m_CmdLine.Command->Push("b"); break;
				case D_KEY_C:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("C"); else m_CmdLine.Command->Push("c"); break;
				case D_KEY_D:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("D"); else m_CmdLine.Command->Push("d"); break;
				case D_KEY_E:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("E"); else m_CmdLine.Command->Push("e"); break;
				case D_KEY_F:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("F"); else m_CmdLine.Command->Push("f"); break;
				case D_KEY_G:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("G"); else m_CmdLine.Command->Push("g"); break;
				case D_KEY_H:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("H"); else m_CmdLine.Command->Push("h"); break;
				case D_KEY_I:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("I"); else m_CmdLine.Command->Push("i"); break;
				case D_KEY_J:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("J"); else m_CmdLine.Command->Push("j"); break;
				case D_KEY_K:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("K"); else m_CmdLine.Command->Push("k"); break;
				case D_KEY_L:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("L"); else m_CmdLine.Command->Push("l"); break;
				case D_KEY_M:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("M"); else m_CmdLine.Command->Push("m"); break;
				case D_KEY_N:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("N"); else m_CmdLine.Command->Push("n"); break;
				case D_KEY_O:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("O"); else m_CmdLine.Command->Push("o"); break;
				case D_KEY_P:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("P"); else m_CmdLine.Command->Push("p"); break;
				case D_KEY_Q:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("Q"); else m_CmdLine.Command->Push("q"); break;
				case D_KEY_R:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("R"); else m_CmdLine.Command->Push("r"); break;
				case D_KEY_S:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("S"); else m_CmdLine.Command->Push("s"); break;
				case D_KEY_T:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("T"); else m_CmdLine.Command->Push("t"); break;
				case D_KEY_U:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("U"); else m_CmdLine.Command->Push("u"); break;
				case D_KEY_V:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("V"); else m_CmdLine.Command->Push("v"); break;
				case D_KEY_W:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("W"); else m_CmdLine.Command->Push("w"); break;
				case D_KEY_X:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("X"); else m_CmdLine.Command->Push("x"); break;
				case D_KEY_Y:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("Y"); else m_CmdLine.Command->Push("y"); break;
				case D_KEY_Z:		if (event.GetMod() == D_MOD_SHIFT || event.GetMod() == D_MOD_CAPS_LOCK) m_CmdLine.Command->Push("Z"); else m_CmdLine.Command->Push("z"); break;
			}
			case D_KEY_BACKSPACE:	m_CmdLine.Command->Pop(); break;
		}

		Gui::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), MAX_TEXT_CHAR);
	}

	void Console::ClickHandle()
	{
		m_Clicked = !m_Clicked;
		m_TexOffset = !m_TexOffset;

		m_CmdLine.Quad.vertices[0].texCoord = glm::vec2(0.0f + float(m_TexOffset) / 4, 0.5f);
		m_CmdLine.Quad.vertices[1].texCoord = glm::vec2(0.25f + float(m_TexOffset) / 4, 0.5f);
		m_CmdLine.Quad.vertices[2].texCoord = glm::vec2(0.25f + float(m_TexOffset) / 4, 0.75f);
		m_CmdLine.Quad.vertices[3].texCoord = glm::vec2(0.0f + float(m_TexOffset) / 4, 0.75f);

		Gui::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);
	}

	void Console::SubmitCommand()
	{
		if (m_Clicked)
		{
			ClickHandle();
			std::string cmd = m_CmdLine.Command->GetText();
			if (m_Command.find(cmd) != m_Command.end())
			{
				m_Command[cmd]();		
				unsigned int oldNum = m_Text->GetNumChar();
				if (oldNum + cmd.size() < MAX_CHAR_PER_CONSOLE)
				{
					m_Text->Push(cmd+"\n");
					m_TextRenderer->PushOffset(m_Text->GetVertice(oldNum),cmd.size(), oldNum);

					if (m_Text->GetSize().y >= m_Size.y - m_CmdLine.Size.y)
					{
						m_Size.y = m_Text->GetSize().y + m_CmdLine.Size.y;
						m_Quad.SetPosition(m_Position, m_Size);
						Gui::Get()->UpdateVertexBuffer(m_Index, &m_Quad);

						m_CmdLine.Position = glm::vec2(m_Position.x, m_Position.y + m_Size.y - m_CmdLine.Size.y);
						m_CmdLine.Quad.SetPosition(m_CmdLine.Position, m_CmdLine.Size);
						Gui::Get()->UpdateVertexBuffer(m_CmdLine.Index, &m_CmdLine.Quad);

						glm::vec2 offset = glm::vec2(5, 2);
						m_CmdLine.Command->SetPosition(m_CmdLine.Position + offset);
						Gui::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), m_CmdLine.Command->GetNumChar());
					}
				}
				else
					LOG_WARN("Conole reached maximum of characters, clear console");
				
			}
			else
				LOG_ERR("Command %s does not exist",cmd);

			m_CmdLine.Command->Clear();
			m_CmdLine.Command->RestartCurserX();
			m_CmdLine.Command->RestartCurserY();
			Gui::Get()->UpdateTextBuffer(m_CmdLine.Index, m_CmdLine.Command->GetVertice(0), m_CmdLine.Command->GetNumChar());
		}
	}

	void Console::Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize, float textSize)
	{
		if (Gui::Get())
		{
			D_ASSERT(label.size() < MAX_CHAR_PER_LABEL, "Max len of label is %d", MAX_CHAR_PER_LABEL);
			Ref<Console> console = std::make_shared<Console>(label, position, size, labelsize, textSize);
			Gui::Get()->AddConsole(label, console);
		}
	}

	glm::vec4 Console::getCoords()
	{
		return glm::vec4(m_Position.x,
			m_Position.y + m_Size.y,
			m_Position.x + m_Size.x,
			m_Position.y);
	}
}