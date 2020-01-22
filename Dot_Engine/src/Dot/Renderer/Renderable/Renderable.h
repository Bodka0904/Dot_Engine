#pragma once

#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Renderer.h"
#include "Dot/Renderer/MaterialStack.h"

#include <glm/glm.hpp>
#include <bitset>

namespace Dot {

	struct Vertex3D
	{
		glm::vec3 position;
		glm::vec2 texcoord;
	};
	struct Quad3D
	{
		Quad3D(const glm::vec3& position, const glm::vec2& size)
		{
			vertex[0].position = glm::vec3(position.x - size.x / 2, position.y - size.y / 2, position.z);
			vertex[1].position = glm::vec3(position.x + size.x / 2, position.y - size.y / 2, position.z);
			vertex[2].position = glm::vec3(position.x + size.x / 2, position.y + size.y / 2, position.z);
			vertex[3].position = glm::vec3(position.x - size.x / 2, position.y + size.y / 2, position.z);

			vertex[0].texcoord = glm::vec2(0, 0);
			vertex[1].texcoord = glm::vec2(1, 0);
			vertex[2].texcoord = glm::vec2(1, 1);
			vertex[3].texcoord = glm::vec2(0, 1);
		}
		Vertex3D vertex[4];
	};

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
		virtual void Render(const Ref<Shader>& shader,int drawMod) = 0;
		virtual Ref<ArrayBuffer>& GetVAO() = 0;
	};

	struct RenderComponent
	{
		Ref<Renderable> renderable;
		MaterialID  materialID;
		int drawMod = D_TRIANGLES;
	};

}