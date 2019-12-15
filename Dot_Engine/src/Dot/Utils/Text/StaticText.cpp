#pragma once
#include "stdafx.h"
#include "StaticText.h"
#include "Font.h"


namespace Dot {
	Ref<ArrayBuffer>		StaticText::s_VAO;
	std::vector <glm::vec2> StaticText::s_Vertice;
	std::vector <glm::vec2> StaticText::s_TexCoord;
	unsigned int			StaticText::s_NumChars = 0;

	StaticText::StaticText(const std::string& font, std::string text, const glm::vec2 position, const glm::vec2 size)
		:
		m_Position(position),
		m_Size(glm::vec2(0, Font::GetFont(font)->GetData().lineHeight* size.y)),
		m_Len(0),
		m_PositionInBuffer(s_NumChars * 4)
	{
		double curserX = position.x;
		double curserY = 0;
		Character tmp;
		for (char& c : text)
		{
			if (c == '\n')
			{
				curserY += m_Size.y; curserX = position.x;
			}
			else
			{
				tmp = Font::GetFont(font)->GetCharacter(c);

				tmp.sizeX *= size.x;
				tmp.xOffset *= size.x;
				tmp.sizeY *= size.y;
				tmp.yOffset *= size.y;

				s_TexCoord.push_back(glm::vec2(tmp.xTextureCoord, tmp.yTextureCoord));
				s_TexCoord.push_back(glm::vec2(tmp.xMaxTextureCoord, tmp.yTextureCoord));
				s_TexCoord.push_back(glm::vec2(tmp.xMaxTextureCoord, tmp.yMaxTextureCoord));
				s_TexCoord.push_back(glm::vec2(tmp.xTextureCoord, tmp.yMaxTextureCoord));


				s_Vertice.push_back(glm::vec2(curserX + tmp.xOffset, curserY + position.y + tmp.yOffset));
				s_Vertice.push_back(glm::vec2(curserX + tmp.sizeX + tmp.xOffset, curserY + position.y + tmp.yOffset));
				s_Vertice.push_back(glm::vec2(curserX + tmp.sizeX + tmp.xOffset, curserY + position.y + tmp.sizeY + tmp.yOffset));
				s_Vertice.push_back(glm::vec2(curserX + tmp.xOffset, curserY + position.y + tmp.sizeY + tmp.yOffset));

				curserX += tmp.xAdvance * size.x;
				m_Size.x += tmp.xAdvance * size.x;
				m_Len++;
				s_NumChars++;
			}
		}

		s_VAO->GetVertexBuffer(0)->Update((void*)& s_Vertice[m_PositionInBuffer], m_Len * sizeof(glm::vec2) * 4, m_PositionInBuffer * sizeof(glm::vec2) );
		s_VAO->GetVertexBuffer(1)->Update((void*)& s_TexCoord[m_PositionInBuffer], m_Len * sizeof(glm::vec2) * 4, m_PositionInBuffer * sizeof(glm::vec2) );
	}
	void StaticText::SetPosition(const glm::vec2& position)
	{
		std::vector<glm::vec2> newVertices;
		newVertices.resize(m_Len * 4);

		for (int i = 0; i < m_Len * 4; ++i)
		{
			s_Vertice[m_PositionInBuffer + i] += (position - m_Position);
			newVertices[i] = s_Vertice[m_PositionInBuffer + i];
		}
		m_Position = position;
		s_VAO->GetVertexBuffer(0)->Update((void*)& newVertices[0], sizeof(glm::vec2) * m_Len * 4, m_PositionInBuffer * sizeof(glm::vec2));
	}

	void StaticText::Init(int numChars)
	{
		s_VAO = ArrayBuffer::Create();
		Ref<VertexBuffer>VBO[2];

		BufferLayout layout = {
				{0,ShaderDataType::Float2,"a_Position"},
		};
		VBO[0] = VertexBuffer::Create((void*)NULL, numChars * sizeof(glm::vec2) * 4, D_DYNAMIC_DRAW);
		VBO[0]->SetLayout(layout);
		s_VAO->AddVBO(VBO[0]);

		BufferLayout layout_tex = {
				{1,ShaderDataType::Float2,"a_TexCoord"},
		};
		VBO[1] = VertexBuffer::Create((void*)NULL, numChars * sizeof(glm::vec2) * 4, D_DYNAMIC_DRAW);
		VBO[1]->SetLayout(layout_tex);
		s_VAO->AddVBO(VBO[1]);
	}
}