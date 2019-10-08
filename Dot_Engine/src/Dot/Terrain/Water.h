#pragma once
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/Graphics/Renderer/Buffers/FrameBuffer.h"

#include <glm/glm.hpp>

namespace Dot {


	class Water
	{
	public:
		Water(const glm::vec3& position,const glm::vec2& size);


		const Ref<ArrayBuffer>& GetVAO() const { return m_VAO; }
	private:
		Ref<ArrayBuffer>m_VAO;
		float m_Height;
		
	};
}