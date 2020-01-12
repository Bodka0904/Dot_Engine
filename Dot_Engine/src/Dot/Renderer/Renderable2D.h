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
		QuadVertex()
		{
			vertices[0].position = glm::vec2(-100.0f);
			vertices[1].position = glm::vec2(-100.0f);
			vertices[2].position = glm::vec2(-100.0f);
			vertices[3].position = glm::vec2(-100.0f);
			
			vertices[0].texCoord = glm::vec2(-100.0f);
			vertices[1].texCoord = glm::vec2(-100.0f);
			vertices[2].texCoord = glm::vec2(-100.0f);
			vertices[3].texCoord = glm::vec2(-100.0f);
			
		}
		QuadVertex(const glm::vec2& position, const glm::vec2& size,const glm::vec2* texCoord)
		{
			vertices[0].position = glm::vec2(position.x, position.y);
			vertices[1].position = glm::vec2(position.x + size.x, position.y);
			vertices[2].position = glm::vec2(position.x + size.x, position.y + size.y);	
			vertices[3].position = glm::vec2(position.x, position.y + size.y);
			if (texCoord != NULL)
			{
				vertices[0].texCoord = texCoord[0];
				vertices[1].texCoord = texCoord[1];
				vertices[2].texCoord = texCoord[2];
				vertices[3].texCoord = texCoord[3];
			}
		}
		void Move(const glm::vec2 dist)
		{
			vertices[0].position += dist;
			vertices[1].position += dist;
			vertices[2].position += dist;
			vertices[3].position += dist;
		}
		void SetPosition(const glm::vec2& pos,const glm::vec2& size)
		{
			vertices[0].position = glm::vec2(pos);
			vertices[1].position = glm::vec2(pos.x + size.x,pos.y);
			vertices[2].position = glm::vec2(pos+size);
			vertices[3].position = glm::vec2(pos.x,pos.y+size.y);
		}
		Vertex vertices[4];
	};
	
	class Renderable2D
	{
	public:
		virtual const QuadVertex& GetQuad() = 0;		
	};

}