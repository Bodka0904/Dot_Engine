#pragma once

#include <glm/glm.hpp>

namespace Dot {
	struct Vertex
	{
		glm::vec2 position;
		glm::vec2 texCoord;
	};
	
	struct QuadVertex
	{
		QuadVertex() {};
		QuadVertex(const glm::vec2& position, const glm::vec2& size,const glm::vec2* texCoord)
		{
			m_Vertices[0].position = glm::vec2(position.x, position.y);
			m_Vertices[1].position = glm::vec2(position.x + size.x, position.y);
			m_Vertices[2].position = glm::vec2(position.x + size.x, position.y + size.y);	
			m_Vertices[3].position = glm::vec2(position.x, position.y + size.y);
			if (texCoord != NULL)
			{
				m_Vertices[0].texCoord = texCoord[0];
				m_Vertices[1].texCoord = texCoord[1];
				m_Vertices[2].texCoord = texCoord[2];
				m_Vertices[3].texCoord = texCoord[3];
			}
		}
		Vertex m_Vertices[4];
	};
	
	class Renderable2D
	{
	public:
		virtual const QuadVertex& GetQuad() = 0;		
	};

}