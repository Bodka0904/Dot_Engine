#include "stdafx.h"
#include "DynamicText.h"

#include "Dot/Debug/Log.h"

#include <GL/glew.h>

namespace Dot {
	DynamicText::DynamicText(const std::string& font, std::string text, const glm::vec2 position, const glm::vec2 size, int MaxCharacter)
		:
		m_Font(font),
		m_Position(position),
		m_Size(size),
		m_NumChars(0),
		m_MaxChar(MaxCharacter)
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

	void DynamicText::Update(std::string text)
	{
		for (char& c : text)
		{
			if (m_PositionInBuffer + 4 >= m_MaxChar)
			{
				LOG_WARN("Buffer for dynamic text is full!");
				break;
			}
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
				if (m_PositionInBuffer >= m_NumChars)
					m_NumChars++;
				
			}
		}
	}

	void DynamicText::SetPositionInBuffer(int offset)
	{
		m_PositionInBuffer = offset;
	}

}
