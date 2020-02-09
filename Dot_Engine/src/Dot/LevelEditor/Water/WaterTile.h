#pragma once
#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include <glm/glm.hpp>


namespace Dot {
	class WaterTile
	{
	public:
		WaterTile(const glm::vec3& position, const glm::vec2& size);

		void Draw();

		Ref<ArrayBuffer> m_VAO;
		std::vector<glm::vec3> m_Positions;

		glm::vec3 m_Position;
		glm::vec2 m_Size;
	};
}