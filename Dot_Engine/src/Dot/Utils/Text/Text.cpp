#include "stdafx.h"
#include "Text.h"
#include "Dot/Debug/Log.h"


namespace Dot {
	Text::Text(const std::string& font, std::string text, const glm::vec2 position, const glm::vec2 size, int maxCharacter, int lineSize)
		:
		m_Font(font),
		m_Position(position),
		m_LineHeight(Font::GetFont(font)->GetData().lineHeight* size.y),
		m_SizeText(glm::vec2(0, Font::GetFont(font)->GetData().lineHeight* size.y)),
		m_Size(size),
		m_MaxChar(maxCharacter),
		m_LineSize(lineSize)
	{
		m_Vertice.resize(m_MaxChar);
		m_CurserX = position.x;
		m_CurserY = 0;

		Character tmp;
		for (char& c : text)
		{
			if (c == '\n')
			{
				m_CurserY += m_LineHeight; 
				m_CurserX = position.x;
				m_SizeText.y += m_LineHeight;
			}
			else
			{
				m_Text += c;
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

				m_Vertice[m_PositionInBuffer] = QuadVertex(glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + position.y + tmp.yOffset),
					glm::vec2(tmp.sizeX, tmp.sizeY), &texCoords[0]);

				m_CurserX += tmp.xAdvance * size.x;
				if (m_CurserX >= m_LineSize+m_Position.x)
				{
					m_CurserY += m_LineHeight;
					m_CurserX = position.x;
					m_SizeText.y += m_LineHeight;
				}
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
		for (char& c : text)
		{
			if (m_PositionInBuffer < m_MaxChar)
			{
				if (c == '\n')
				{
					m_CurserY += m_LineHeight;
					m_CurserX = m_Position.x;
					m_SizeText.y += m_LineHeight;
				}
				else
				{
					m_Text += c;
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
					if (m_CurserX >= m_LineSize + m_Position.x)
					{
						m_CurserY += m_LineHeight;
						m_CurserX = m_Position.x;
						m_SizeText.y += m_LineHeight;
					}

					m_PositionInBuffer++;
				}
			}
			else
			{
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
			m_Text.pop_back();
		}
	}

	void Text::Clear()
	{
		m_PositionInBuffer = 0;
		m_CurserX = m_Position.x;
		m_CurserY = 0;
		m_Vertice.clear();
		m_Text.clear();
		m_Vertice.resize(m_MaxChar);
		m_SizeText.y = m_LineHeight;
		for (int i = 0; i < m_MaxChar;++i)
		{
			m_Vertice[i] = QuadVertex();
		}
	}

	void Text::SetPositionInBuffer(int offset)
	{
		m_PositionInBuffer = offset;
		m_Text = m_Text.substr(0, offset);
	}

}
