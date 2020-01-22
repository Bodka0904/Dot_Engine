#pragma once

#include <glm/glm.hpp>

namespace Dot {
	struct Vertex2D
	{
		glm::vec4 color;
		glm::vec2 position;
		glm::vec2 texCoord;
	};

	struct QuadVertex2D
	{
		QuadVertex2D();
		QuadVertex2D(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, const glm::vec2* texCoord);

		void Move(const glm::vec2 dist);
		void SetPosition(const glm::vec2& pos, const glm::vec2& size);
		void SetColor(const glm::vec3& color);

		Vertex2D vertices[4];
	};
	class Renderable2D
	{
	public:
		virtual const QuadVertex2D& GetQuad() = 0;		
	};


	
}