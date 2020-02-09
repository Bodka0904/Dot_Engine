#include "stdafx.h"
#include "Text.h"
#include "Dot/Debug/Log.h"


namespace Dot {
	Text::Text(const std::string& font, std::string text, const glm::vec2 position, const glm::vec2 size, const glm::vec3& color, unsigned int maxCharacter, unsigned int numCharRow)
		:
		m_Font(font),
		m_Position(position),
		m_LineHeight(float(Font::GetFont(font)->GetData().lineHeight) * size.y),
		m_Size(glm::vec2(0, float(Font::GetFont(font)->GetData().lineHeight)* size.y)),
		m_CharSize(size),
		m_MaxChar(maxCharacter),
		m_NumCharPerRow(numCharRow),
		m_PositionInBuffer(0),
		m_CurrentCharInRow(0)
	{
		m_Vertice.resize(m_MaxChar);
		m_CurserX = position.x;
		m_CurserY = 0;

		Character tmp;
		for (char& c : text)
		{
			if (c == '\n' || m_CurrentCharInRow == m_NumCharPerRow)
			{
				m_CurserY += m_LineHeight;
				m_CurserX = position.x;
				m_Size.y += m_LineHeight;
				m_CurrentCharInRow = 0;
			}
			else
			{
				m_Text += c;
				tmp = Font::GetFont(font)->GetCharacter(c);

				tmp.sizeX *= m_CharSize.x;
				tmp.xOffset *= m_CharSize.x;
				tmp.sizeY *= m_CharSize.y;
				tmp.yOffset *= m_CharSize.y;

				glm::vec2 texCoords[4] = {
					glm::vec2(tmp.xTextureCoord, tmp.yTextureCoord),
					glm::vec2(tmp.xMaxTextureCoord, tmp.yTextureCoord),
					glm::vec2(tmp.xMaxTextureCoord, tmp.yMaxTextureCoord),
					glm::vec2(tmp.xTextureCoord, tmp.yMaxTextureCoord)
				};

				m_Vertice[m_PositionInBuffer] = QuadVertex2D(glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + position.y + tmp.yOffset),
					glm::vec2(tmp.sizeX, tmp.sizeY), color, &texCoords[0]);

				m_CurserX += float(tmp.xAdvance) * m_CharSize.x;
				m_Size.x += float(tmp.xAdvance) * m_CharSize.x;
				m_PositionInBuffer++;
				m_CurrentCharInRow++;

			}
		}

	}

	void Text::Push(std::string text, const glm::vec3& color)
	{
		for (char& c : text)
		{
			if (m_PositionInBuffer < m_MaxChar)
			{
				if (c == '\n' || m_CurrentCharInRow == m_NumCharPerRow)
				{
					m_CurserY += m_LineHeight;
					m_CurserX = m_Position.x;
					m_Size.y += m_LineHeight;
					m_CurrentCharInRow = 0;
				}
				else
				{
					m_Text += c;
					Character tmp = Font::GetFont(m_Font)->GetCharacter(c);

					tmp.sizeX *= m_CharSize.x;
					tmp.xOffset *= m_CharSize.x;
					tmp.sizeY *= m_CharSize.y;
					tmp.yOffset *= m_CharSize.y;

					glm::vec2 texCoords[4] = {
						glm::vec2(tmp.xTextureCoord, tmp.yTextureCoord),
						glm::vec2(tmp.xMaxTextureCoord, tmp.yTextureCoord),
						glm::vec2(tmp.xMaxTextureCoord, tmp.yMaxTextureCoord),
						glm::vec2(tmp.xTextureCoord, tmp.yMaxTextureCoord)
					};

					m_Vertice[m_PositionInBuffer] = QuadVertex2D(glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + m_Position.y + tmp.yOffset),
						glm::vec2(tmp.sizeX, tmp.sizeY), color, &texCoords[0]);

					m_CurserX += float(tmp.xAdvance) * m_CharSize.x;
					m_Size.x += float(tmp.xAdvance) * m_CharSize.x;

					m_PositionInBuffer++;
					m_CurrentCharInRow++;
				}
			}
			else
			{
				LOG_WARN("Buffer for dynamic text is full!");
				break;
			}
		}
	}

	void Text::PushCharacter(const char character, const glm::vec3& color)
	{
		if (m_PositionInBuffer < m_MaxChar)
		{
			if (character == '\n' || m_CurrentCharInRow == m_NumCharPerRow)
			{
				m_CurserY += m_LineHeight;
				m_CurserX = m_Position.x;
				m_Size.y += m_LineHeight;

				m_CurrentCharInRow = 0;
			}
			else
			{
				m_Text += character;
				Character tmp = Font::GetFont(m_Font)->GetCharacter(character);

				tmp.sizeX *= m_CharSize.x;
				tmp.xOffset *= m_CharSize.x;
				tmp.sizeY *= m_CharSize.y;
				tmp.yOffset *= m_CharSize.y;

				glm::vec2 texCoords[4] = {
					glm::vec2(tmp.xTextureCoord, tmp.yTextureCoord),
					glm::vec2(tmp.xMaxTextureCoord, tmp.yTextureCoord),
					glm::vec2(tmp.xMaxTextureCoord, tmp.yMaxTextureCoord),
					glm::vec2(tmp.xTextureCoord, tmp.yMaxTextureCoord)
				};

				m_Vertice[m_PositionInBuffer] = QuadVertex2D(glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + m_Position.y + tmp.yOffset),
					glm::vec2(tmp.sizeX, tmp.sizeY), color, &texCoords[0]);

				m_CurserX += float(tmp.xAdvance) * m_CharSize.x;
				m_Size.x += float(tmp.xAdvance) * m_CharSize.x;
				m_PositionInBuffer++;
				m_CurrentCharInRow++;
			}
		}
		else
		{
			LOG_WARN("Buffer for dynamic text is full!");
		}
	}

	void Text::Pop()
	{
		if (m_PositionInBuffer > 0)
		{
			m_Vertice[m_PositionInBuffer - 1] = QuadVertex2D(glm::vec2(0), glm::vec2(0), glm::vec3(1, 1, 1), NULL);
			Character tmp = Font::GetFont(m_Font)->GetCharacter(m_Text.c_str()[m_PositionInBuffer - 1]);
			m_CurserX -= float(tmp.xAdvance) * m_Size.x;
			m_Size.x -= float(tmp.xAdvance) * m_Size.x;
			m_PositionInBuffer--;
			m_Text.pop_back();
		}
	}

	void Text::Clear()
	{
		m_PositionInBuffer = 0;
		m_Size.x = 0;
		m_CurserX = m_Position.x;
		m_CurserY = 0;
		m_Vertice.clear();
		m_Text.clear();
		m_Vertice.resize(m_MaxChar);
		m_Size.y = m_LineHeight;
		for (int i = 0; i < (int)m_MaxChar; ++i)
		{
			m_Vertice[i] = QuadVertex2D();
		}
	}

	void Text::RestartCurserX()
	{
		m_CurserX = m_Position.x;
	}

	void Text::RestartCurserY()
	{
		m_CurserY = 0;
	}

	void Text::SetPosition(const glm::vec2& pos)
	{
		for (size_t i = 0; i < m_PositionInBuffer; ++i)
		{
			for (size_t j = 0; j < sizeof(m_Vertice[i].vertices) / sizeof(Vertex2D); ++j)
			{
				m_Vertice[i].vertices[j].position += (pos - m_Position);
			}
		}
		m_CurserX += pos.x - m_Position.x;
		m_Position = pos;
	}

	void Text::Move(const glm::vec2& pos)
	{
		m_Position += pos;

		for (size_t i = 0; i < m_PositionInBuffer; ++i)
		{
			for (size_t j = 0; j < sizeof(m_Vertice[i].vertices) / sizeof(Vertex2D); ++j)
			{
				m_Vertice[i].vertices[j].position += pos;
			}
		}
		m_CurserX += pos.x;
	}

}
