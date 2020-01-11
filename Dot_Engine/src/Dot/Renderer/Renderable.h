#pragma once

#include "Buffers/ArrayBuffer.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Material.h"

#include <glm/glm.hpp>
#include <bitset>

namespace Dot {
	struct StaticVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Binormal;
		glm::vec2 Texcoord;
	};

	static_assert(sizeof(StaticVertex) == 14 * sizeof(float));
	static const int NumAttributes = 5;

	struct Index
	{
		uint32_t V1, V2, V3;
	};
	static_assert(sizeof(Index) == 3 * sizeof(uint32_t));


	class Renderable
	{
	public:
		virtual void Render(const Ref<Shader>& shader) = 0;
	};

	struct RenderComponent
	{
		Ref<Renderable> mesh;
		Ref<Material> material;
	};

}