#include "stdafx.h"
#include "Particle.h"
#include <GL/glew.h>


#ifdef D_CUDA
namespace Dot {
	ParticleEngine::ParticleEngine(unsigned int count)
		:m_count(count)
	{
		//std::vector<glm::vec3> vertices;
		//vertices.push_back(glm::vec3(0, 0, 0));
	//	m_VAO = std::make_shared<ArrayBuffer>();

		//BufferLayout layout = {
		//	{0,ShaderDataType::Float3,"positions",1},
		//};
		//Ref<VertexBuffer>m_VBO;

		//m_VBO = std::make_shared<VertexBuffer>((void*)NULL, m_count * sizeof(float3), D_DYNAMIC_DRAW);
		//m_VBO->SetLayout(layout);

		//m_VAO->AddVBO(m_VBO);

		//m_effect = std::make_shared<DefaultParticleEffect>(m_count);

		TEST = std::make_shared<TestParticleEffect>(m_count);
	}
	ParticleEngine::~ParticleEngine()
	{
	
	}
	void ParticleEngine::Update(float dt)
	{
		glPointSize(20);
		TEST->Render(dt);
		//m_effect->Launch(dt);
		//m_VAO->GetVertexBuffer(0)->Update(m_effect->m_positions, m_count * sizeof(float3), 0);
	}
}

#endif