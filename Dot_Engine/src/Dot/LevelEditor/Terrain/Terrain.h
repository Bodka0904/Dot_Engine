#pragma once
#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/Renderer/Renderable/StaticMesh.h"
#include "Dot/ECS/ECSManager.h"
#include <glm/glm.hpp>

namespace Dot {
	class TerrainEditor;
	class Terrain : public Renderable
	{
		friend class TerrainEditor;
	public:
		Terrain(float size, int numvertex);
		~Terrain();

		virtual void Render(const Ref<Shader>& shader, int drawMod) override;

		float GetSize() const { return m_Size; }
		float GetHeight(const glm::vec3& position);
		virtual Ref<ArrayBuffer>& GetVAO() { return m_VAO; }
	private:
		float barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
	
	private:
		Ref<ArrayBuffer> m_VAO;
		std::vector<glm::vec4> m_Vertices;
		std::vector<glm::vec4> m_Normals;
		std::vector<glm::vec2> m_TexCoords;
		std::vector<std::vector<float> > m_Heights;

		int m_NumVertex = 0;
		float m_Size = 0.0f;			
	};

}

