#include "stdafx.h"
#include "Text.h"
#include "Dot/Debug/Log.h"


namespace Dot {
	Text::Text(const std::string& font, std::string text, const glm::vec2 position, const glm::vec2 size, int maxCharacter)
		:
		m_Font(font),
		m_Position(position),
		m_SizeText(glm::vec2(0, Font::GetFont(font)->GetData().lineHeight* size.y)),
		m_Size(size),
		m_NumChars(0),
		m_MaxChar(maxCharacter),
		m_Text(text)
	{
		m_Vertice.resize(m_MaxChar);
		m_CurserX = position.x;
		m_CurserY = 0;

		Character tmp;
		for (char& c : text)
		{
			if (c == '\n')
			{
				m_CurserY += m_Size.y; m_CurserX = position.x;
			}
			else
			{
				tmp = Font::GetFont(font)->GetCharacter(c);

				tmp.sizeX *= size.x;
				tmp.xOffset *= size.x;
				tmp.sizeY *= size.y;
				tmp.yOffset *= size.y;

				glm::vec2 texCoords[4] = {
					glm::vec2(tmp.xTextureCoord, tmp.yTextureCoord),
					glm::vec2(tmp.xMaxTextureCoord, tmp.yTextureCoord),
					glm::vec2(tmp.xMaxTextureCoord, tmp.yMaxTextureCoord),
					glm::vec2(tmp.xTextureCoord, tmp.yMaxTextureCoord)
				};

				m_Vertice[m_NumChars] = QuadVertex(glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + position.y + tmp.yOffset),
					glm::vec2(tmp.sizeX, tmp.sizeY), &texCoords[0]);

				m_CurserX += tmp.xAdvance * size.x;
				m_NumChars++;
				m_PositionInBuffer++;
			}
		}

	}

	void Text::SetPosition(const glm::vec2 pos)
	{
		for (size_t i = 0; i < m_PositionInBuffer; ++i)
		{
			for (size_t j = 0; j < sizeof(m_Vertice[i].vertices) / sizeof(Vertex); ++j)
			{
				m_Vertice[i].vertices[j].position += (pos - m_Position);
			}
		}
		m_CurserX += pos.x - m_Position.x;
		m_Position = pos;
	}

	void Text::Push(std::string text)
	{
		m_Text.append(text);
		for (char& c : text)
		{
			if (m_PositionInBuffer < m_MaxChar)
			{
				if (c == '\n')
				{
					m_CurserY += m_Size.y;
				}
				else
				{
					Character tmp = Font::GetFont(m_Font)->GetCharacter(c);

					tmp.sizeX *= m_Size.x;
					tmp.xOffset *= m_Size.x;
					tmp.sizeY *= m_Size.y;
					tmp.yOffset *= m_Size.y;

					glm::vec2 texCoords[4] = {
						glm::vec2(tmp.xTextureCoord, tmp.yTextureCoord),
						glm::vec2(tmp.xMaxTextureCoord, tmp.yTextureCoord),
						glm::vec2(tmp.xMaxTextureCoord, tmp.yMaxTextureCoord),
						glm::vec2(tmp.xTextureCoord, tmp.yMaxTextureCoord)
					};

					m_Vertice[m_PositionInBuffer] = QuadVertex(glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + m_Position.y + tmp.yOffset),
						glm::vec2(tmp.sizeX, tmp.sizeY), &texCoords[0]);

					m_CurserX += tmp.xAdvance * m_Size.x;
					m_PositionInBuffer++;

					if (m_PositionInBuffer > m_NumChars)
						m_NumChars++;
				}
			}
			else
			{
				m_NumChars = m_MaxChar - 1;
				LOG_WARN("Buffer for dynamic text is full!");
				break;
			}
		}
	}

	void Text::Pop()
	{
		if (m_PositionInBuffer > 0)
		{
			m_Vertice[m_PositionInBuffer - 1] = QuadVertex(glm::vec2(0), glm::vec2(0), NULL);
			Character tmp = Font::GetFont(m_Font)->GetCharacter(m_Text.c_str()[m_PositionInBuffer - 1]);
			m_CurserX -= tmp.xAdvance * m_Size.x;
			m_PositionInBuffer--;
			m_NumChars--;
			m_Text.pop_back();
		}
	}

	void Text::Clear()
	{
		m_PositionInBuffer = 0;
		m_CurserX = m_Position.x;
		m_Vertice.clear();
		for (int i = 0; i < m_MaxChar;++i)
		{
			m_Vertice.push_back(QuadVertex(glm::vec2(0),glm::vec2(0),NULL));
		}
	}

	void Text::SetPositionInBuffer(int offset)
	{
		m_PositionInBuffer = offset;
		m_Text = m_Text.substr(0, offset);
	}

}
