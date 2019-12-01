#pragma once
#include "stdafx.h"
#include "Text.h"

#include <GL/glew.h>

namespace Dot {
	Text::Text(Font& font, std::string text, const glm::vec2& position, const glm::vec2& size)
	{
		double curser = position.x;

		Character tmp;
		for (char& c : text) 
		{
			tmp = font.GetCharacter(c);
			
			tmp.sizeX *= size.x;
			tmp.xOffset *= size.x;
			tmp.sizeY *= size.y;
			tmp.yOffset *= size.y;

			m_TexCoord.push_back(tmp.xTextureCoord);
			m_TexCoord.push_back(tmp.yTextureCoord);

			m_TexCoord.push_back(tmp.xMaxTextureCoord);
			m_TexCoord.push_back(tmp.yTextureCoord);

			m_TexCoord.push_back(tmp.xMaxTextureCoord);
			m_TexCoord.push_back(tmp.yMaxTextureCoord);

			m_TexCoord.push_back(tmp.xTextureCoord);
			m_TexCoord.push_back(tmp.yMaxTextureCoord);

			m_Vertice.push_back(curser+tmp.xOffset);
			m_Vertice.push_back(position.y+tmp.yOffset);

			m_Vertice.push_back(curser + tmp.sizeX + tmp.xOffset);
			m_Vertice.push_back(position.y +tmp.yOffset);

			m_Vertice.push_back(curser + tmp.sizeX + tmp.xOffset);
			m_Vertice.push_back(position.y + tmp.sizeY + tmp.yOffset);

			m_Vertice.push_back(curser + tmp.xOffset);
			m_Vertice.push_back(position.y + tmp.sizeY + tmp.yOffset);

			curser += tmp.xAdvance*size.x;
		}

		m_VAO = std::make_shared<ArrayBuffer>();
		Ref<VertexBuffer>m_VBO[2];

		BufferLayout layout = {
				{0,ShaderDataType::Float2,"a_Position"},
		};
		m_VBO[0] = std::make_shared<VertexBuffer>((void*)& m_Vertice[0], m_Vertice.size() * sizeof(float), D_STATIC_DRAW);
		m_VBO[0]->SetLayout(layout);
		m_VAO->AddVBO(m_VBO[0]);

		BufferLayout layout_tex = {
				{1,ShaderDataType::Float2,"a_TexCoord"},
		};
		m_VBO[1] = std::make_shared<VertexBuffer>(&m_TexCoord[0], m_TexCoord.size() * sizeof(float), D_STATIC_DRAW);
		m_VBO[1]->SetLayout(layout_tex);
		m_VAO->AddVBO(m_VBO[1]);

	}
	void Text::RenderText()
	{
		m_VAO->Bind();
		glDrawArrays(GL_QUADS, 0, m_VAO->GetVertexBuffer(0)->GetCount());
	}
}