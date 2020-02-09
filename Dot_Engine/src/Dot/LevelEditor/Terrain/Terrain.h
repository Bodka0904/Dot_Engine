#pragma once
#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/ECS/ECSManager.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Renderable/Renderable.h"
#include <glm/glm.hpp>

namespace Dot {
	class TerrainCollider
	{
	public:
		TerrainCollider();
		
		float GetHeight(const glm::vec3& position);
		void UpdateHeights(Entity entity);

		std::vector<std::vector<float> > heights;
		int numVertex = 0;
		float size = 0.0f;

	private:
		float barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
	};

	class Terrain : public Renderable
	{
	public:
		Terrain(float Size,int NumVertex);
		~Terrain();

		virtual void Render(const Ref<Shader>& shader, int drawMod) override;

		Ref<ArrayBuffer> vao;
		std::vector<glm::vec4> vertices;
		std::vector<glm::vec4> normals;
		std::vector<glm::vec2> texCoords;

		int numVertex;
		float size;
	};

}
