#pragma once
#include "Renderable.h"
#include "Dot/PhysicsEngine/Components/ComponentData.h"

namespace Dot {

	class InstancedMesh : public Renderable
	{
	public:
		InstancedMesh(const std::string& filename, int num);

		void UpdateBuffer();
		virtual void Render(const Ref<Shader>& shader, int drawMod) override;

		Ref<ArrayBuffer> vao;
		std::vector<StaticVertex> vertices;
		std::vector<Index> indices;
		std::vector<glm::mat4> models;
		
		unsigned int numVertices;
		unsigned int capacity;
		unsigned int instances;
	};
	
}