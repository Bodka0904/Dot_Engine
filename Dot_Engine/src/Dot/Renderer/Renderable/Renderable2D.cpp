#pragma once
#include "stdafx.h"
#include "Renderable2D.h"

namespace Dot {
	QuadVertex2D::QuadVertex2D()
	{
		vertices[0].position = glm::vec2(-100.0f);
		vertices[1].position = glm::vec2(-100.0f);
		vertices[2].position = glm::vec2(-100.0f);
		vertices[3].position = glm::vec2(-100.0f);

		vertices[0].texCoord = glm::vec2(-100.0f);
		vertices[1].texCoord = glm::vec2(-100.0f);
		vertices[2].texCoord = glm::vec2(-100.0f);
		vertices[3].texCoord = glm::vec2(-100.0f);

		vertices[0].color = glm::vec4(0);
		vertices[1].color = glm::vec4(0);
		vertices[2].color = glm::vec4(0);
		vertices[3].color = glm::vec4(0);
	}
	QuadVertex2D::QuadVertex2D(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, const glm::vec2* texCoord)
	{
		vertices[0].position = glm::vec2(position.x, position.y);
		vertices[1].position = glm::vec2(position.x + size.x, position.y);
		vertices[2].position = glm::vec2(position.x + size.x, position.y + size.y);
		vertices[3].position = glm::vec2(position.x, position.y + size.y);
		
		vertices[0].color = glm::vec4(color,1);
		vertices[1].color = glm::vec4(color,1);
		vertices[2].color = glm::vec4(color,1);
		vertices[3].color = glm::vec4(color,1);

		if (texCoord != NULL)
		{
			vertices[0].texCoord = texCoord[0];
			vertices[1].texCoord = texCoord[1];
			vertices[2].texCoord = texCoord[2];
			vertices[3].texCoord = texCoord[3];
		}
	}
	void QuadVertex2D::Move(const glm::vec2 dist)
	{
		vertices[0].position += dist;
		vertices[1].position += dist;
		vertices[2].position += dist;
		vertices[3].position += dist;
	}
	void QuadVertex2D::SetPosition(const glm::vec2& pos, const glm::vec2& size)
	{
		vertices[0].position = glm::vec2(pos);
		vertices[1].position = glm::vec2(pos.x + size.x, pos.y);
		vertices[2].position = glm::vec2(pos + size);
		vertices[3].position = glm::vec2(pos.x, pos.y + size.y);
	}
	void QuadVertex2D::SetColor(const glm::vec3& color)
	{
		vertices[0].color = glm::vec4(color, 1);
		vertices[1].color = glm::vec4(color, 1);
		vertices[2].color = glm::vec4(color, 1);
		vertices[3].color = glm::vec4(color, 1);
	}
}