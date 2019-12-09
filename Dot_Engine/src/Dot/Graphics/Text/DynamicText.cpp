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
		m_Vertice.resize(m_MaxChar * 4);
		m_TexCoord.resize(m_MaxChar * 4);

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

				m_TexCoord[(m_PositionInBuffer*4)+0] = glm::vec2(tmp.xTextureCoord, tmp.yTextureCoord);
				m_TexCoord[(m_PositionInBuffer*4)+1] = glm::vec2(tmp.xMaxTextureCoord, tmp.yTextureCoord);
				m_TexCoord[(m_PositionInBuffer*4)+2] = glm::vec2(tmp.xMaxTextureCoord, tmp.yMaxTextureCoord);
				m_TexCoord[(m_PositionInBuffer*4)+3] = glm::vec2(tmp.xTextureCoord, tmp.yMaxTextureCoord);

				m_Vertice[(m_PositionInBuffer * 4) + 0] = glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + position.y + tmp.yOffset);
				m_Vertice[(m_PositionInBuffer * 4) + 1] = glm::vec2(m_CurserX + tmp.sizeX + tmp.xOffset, m_CurserY + position.y + tmp.yOffset);
				m_Vertice[(m_PositionInBuffer * 4) + 2] = glm::vec2(m_CurserX + tmp.sizeX + tmp.xOffset, m_CurserY + position.y + tmp.sizeY + tmp.yOffset);
				m_Vertice[(m_PositionInBuffer * 4) + 3] = glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + position.y + tmp.sizeY + tmp.yOffset);

				m_CurserX += tmp.xAdvance * size.x;
				
				m_NumChars++;
				m_PositionInBuffer++;
			}
		}

		m_VAO = std::make_shared<ArrayBuffer>();
		Ref<VertexBuffer>VBO[2];

		BufferLayout layout = {
				{0,ShaderDataType::Float2,"a_Position"},
		};
		VBO[0] = std::make_shared<VertexBuffer>((void*)& m_Vertice[0], m_MaxChar * sizeof(glm::vec2) * 4, D_DYNAMIC_DRAW);
		VBO[0]->SetLayout(layout);
		m_VAO->AddVBO(VBO[0]);

		BufferLayout layout_tex = {
				{1,ShaderDataType::Float2,"a_TexCoord"},
		};
		VBO[1] = std::make_shared<VertexBuffer>(&m_TexCoord[0], m_MaxChar * sizeof(glm::vec2) * 4, D_DYNAMIC_DRAW);
		VBO[1]->SetLayout(layout_tex);
		m_VAO->AddVBO(VBO[1]);
	}

	void DynamicText::Update(std::string text)
	{
		int oldLen = m_PositionInBuffer;
		int newLen = 0;
		for (char& c : text)
		{
			if (m_PositionInBuffer >= m_MaxChar)
			{
				LOG_WARN("Buffer for dynamic text is full!\n");
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

				m_TexCoord[(m_PositionInBuffer * 4) + 0] = glm::vec2(tmp.xTextureCoord, tmp.yTextureCoord);
				m_TexCoord[(m_PositionInBuffer * 4) + 1] = glm::vec2(tmp.xMaxTextureCoord, tmp.yTextureCoord);
				m_TexCoord[(m_PositionInBuffer * 4) + 2] = glm::vec2(tmp.xMaxTextureCoord, tmp.yMaxTextureCoord);
				m_TexCoord[(m_PositionInBuffer * 4) + 3] = glm::vec2(tmp.xTextureCoord, tmp.yMaxTextureCoord);

				m_Vertice[(m_PositionInBuffer * 4) + 0] = glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + m_Position.y + tmp.yOffset);
				m_Vertice[(m_PositionInBuffer * 4) + 1] = glm::vec2(m_CurserX + tmp.sizeX + tmp.xOffset, m_CurserY + m_Position.y + tmp.yOffset);
				m_Vertice[(m_PositionInBuffer * 4) + 2] = glm::vec2(m_CurserX + tmp.sizeX + tmp.xOffset, m_CurserY + m_Position.y + tmp.sizeY + tmp.yOffset);
				m_Vertice[(m_PositionInBuffer * 4) + 3] = glm::vec2(m_CurserX + tmp.xOffset, m_CurserY + m_Position.y + tmp.sizeY + tmp.yOffset);
				
				m_CurserX += tmp.xAdvance * m_Size.x;
				newLen++;
				
				m_NumChars++;
				m_PositionInBuffer++;
			}
		}
		m_VAO->GetVertexBuffer(0)->Update((void*)&m_Vertice[oldLen*4],newLen * 4 * sizeof(glm::vec2),oldLen*sizeof(glm::vec2)*4);
		m_VAO->GetVertexBuffer(1)->Update((void*)& m_TexCoord[oldLen*4], newLen * 4 * sizeof(glm::vec2), oldLen * sizeof(glm::vec2) * 4);
	}

	void DynamicText::SetPositionInBuffer(int offset)
	{
		m_PositionInBuffer = offset;
	}

}