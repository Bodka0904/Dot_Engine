#pragma once
#include "Dot/Renderer/Renderable/StaticMesh.h"
#include "Light.h"

namespace Dot {
	class LightController
	{
	public:
		LightController(const glm::vec3& position,const glm::vec3& color);
		void Update(float dt);

		const glm::mat4& GetModel();
		const Ref<ArrayBuffer>& GetVAO()const { return m_Mesh->GetVAO(); }
		const Ref<Light>& GetLight()const { return m_Light; }
	private:
		Ref<StaticMesh> m_Mesh;
		Ref<Light> m_Light;

		glm::mat4 m_Model;

		float m_Speed = 10;
	};
}