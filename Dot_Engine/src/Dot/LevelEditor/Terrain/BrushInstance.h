#pragma once
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Mesh.h"

namespace Dot {
	class BrushInstance
	{
	public:
		BrushInstance(const std::string& model,const std::string texture,int capacity);
		void Update(const std::vector<glm::mat4>& transforms);

		std::vector<Ref<InstancedMesh>>& GetInstances() { return m_Instance; }
	private:
		unsigned int m_CapacityPerInstance;
		unsigned int m_CurrentCount;
		std::vector<Ref<InstancedMesh>> m_Instance;
		Ref<Texture2D> m_Texture;

		std::string m_Model;
	};
}