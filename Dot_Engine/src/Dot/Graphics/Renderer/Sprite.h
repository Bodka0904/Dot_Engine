#pragma once
#include <glm/glm.hpp>

#include "Buffers/ArrayBuffer.h"

namespace Dot {

	//struct Vertex
	//{
	//	glm::vec2 position;
	//	glm::vec2 texcoord;
	//};
	//struct Quad
	//{
	//	Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec2* texcoord)
	//	{
	//
	//		m_Vertices[0].position = glm::vec2(position.x, position.y);
	//		m_Vertices[0].texcoord = texcoord[0];
	//		
	//		m_Vertices[1].position = glm::vec2(position.x + size.x, position.y);
	//		m_Vertices[1].texcoord = texcoord[1];
	//		
	//		m_Vertices[2].position = glm::vec2(position.x + size.x, position.y + size.y);
	//		m_Vertices[2].texcoord = texcoord[2];
	//		
	//		m_Vertices[3].position = glm::vec2(position.x, position.y + size.y);
	//		m_Vertices[3].texcoord = texcoord[3];
	//	}
	//	Vertex m_Vertices[4];
	//};
	//
	//class Sprite
	//{
	//public:
	//	Sprite(const glm::vec2& position,const glm::vec2& size,const void* texcoords);
	//
	//private:
	//	unsigned int m_ID;
	//};
	//
	//
	//class SpriteStack
	//{
	//public:
	//
	//
	//private:
	//	std::vector<ArrayBuffer> m_VAO;
	//
	//	static unsigned int m_NumSprites;
	//
	//};
}