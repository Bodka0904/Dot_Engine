#pragma once
#include "Renderable.h"

namespace Dot {
	class InstancedMesh : public Renderable
	{
	public:
		InstancedMesh(const std::string& filename, int num, const std::vector<glm::mat4>& transforms);
		void Update(const std::vector<glm::mat4>& transforms, unsigned int numInstances, unsigned int offsetInstances);

		virtual void Render(const Ref<Shader>& shader, int drawMod) override;

		const inline unsigned int GetNumVertices() const { return m_NumVertices; }
		const inline unsigned int GetCapacity() const { return m_Capacity; }
		const inline unsigned int GetNumInstance() const { return m_Instances; }
		const inline Ref<ArrayBuffer>& GetMesh() { return m_VAO; }
	private:
		Ref<ArrayBuffer> m_VAO;

		std::vector<StaticVertex> m_Vertices;
		std::vector<Index> m_Indices;
		unsigned int m_NumVertices;

		unsigned int m_Capacity;
		unsigned int m_Instances;
	};
}