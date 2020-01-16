#include "stdafx.h"
#include "ParticleEffect.h"

#include "Dot/Utils/Random/RandomGenerator.h"
#include "Dot/Renderer/RenderCommand.h"

namespace Dot {

	ParticleEffect::ParticleEffect(const ParticleComponent& component)
	{
		if (component.maxParticles > MAX_PARTICLES)
		{
			LOG_WARN("Max number of particles is %d", MAX_PARTICLES);
			m_MaxParticles = MAX_PARTICLES;
		}
		else if (component.maxParticles > m_MaxParticles)
		{
			m_MaxParticles = component.maxParticles;
		}
		m_ParticlesRender = component.numParticles;
		
		m_VAO[0] = ArrayBuffer::Create();
		m_VAO[1] = ArrayBuffer::Create();

		std::vector<ParticleVertexData> m_VertexData;
		std::vector<ParticleAttribute> m_Attribute;

		m_VertexData.resize(m_MaxParticles);
		m_Attribute.resize(m_MaxParticles);

		for (size_t i = 0; i < m_MaxParticles; ++i)
		{
			m_Attribute[i].velocity.x = (float)RandomGenerator::Random(-2, 2);
			m_Attribute[i].velocity.y = (float)RandomGenerator::Random(4, 7);
			m_Attribute[i].velocity.z = (float)RandomGenerator::Random(-2, 2);
			m_Attribute[i].velocity.w = 0;

			m_Attribute[i].sizeOverLifeTime = component.sizeOverLifeTime;
			m_Attribute[i].lifeSpan = (float)RandomGenerator::Random(2, 3);
			m_Attribute[i].gravity = 0;

			m_VertexData[i].position = glm::vec4(0, 0, 0, 1);
		}


		Quad3D quad(glm::vec3(0, 0, 0), glm::vec2(1, 1));
		BufferLayout vbolayout = {
			{1,ShaderDataType::Float3,"position"},
			{2,ShaderDataType::Float2,"texcoord"}
		};
		Ref<VertexBuffer>VBO = VertexBuffer::Create((void*)& quad.vertex[0], sizeof(Vertex3D) * 4, D_DYNAMIC_DRAW);
		VBO->SetLayout(vbolayout);

		m_VAO[0]->AddVBO(VBO);
		m_VAO[1]->AddVBO(VBO);


		BufferLayout buflayout = {
			{0, ShaderDataType::Float4, "offset", 1},
		};
		m_Buffers[0] = std::make_shared<ParticleBuffer>(0, 2, (void*)& m_VertexData[0], m_VertexData.size() * sizeof(ParticleVertexData), buflayout, true);

		m_VAO[0]->AddSSBO(m_Buffers[0]->m_Buffer[0]);
		m_VAO[1]->AddSSBO(m_Buffers[0]->m_Buffer[1]);

		m_Buffers[1] = std::make_shared<ParticleBuffer>(1, 3, (void*)& m_Attribute[0], m_Attribute.size() * sizeof(ParticleAttribute));

		unsigned int indices[6] = {
			0,1,2,0,2,3
		};

		Ref<IndexBuffer> IBO = IndexBuffer::Create(&indices[0], 6);
		m_VAO[0]->AddIBO(IBO);
		Ref<IndexBuffer> IBO2 = IndexBuffer::Create(&indices[0], 6);
		m_VAO[1]->AddIBO(IBO);
	}
	void ParticleEffect::SwapBuffers()
	{
		m_Buffers[0]->m_Buffer[!m_OutputIdx]->BindRange(m_Buffers[0]->m_FirstBindPoint);
		m_Buffers[0]->m_Buffer[m_OutputIdx]->BindRange(m_Buffers[0]->m_SecondBindPoint);

		m_Buffers[1]->m_Buffer[!m_OutputIdx]->BindRange(m_Buffers[1]->m_FirstBindPoint);
		m_Buffers[1]->m_Buffer[m_OutputIdx]->BindRange(m_Buffers[1]->m_SecondBindPoint);

		m_OutputIdx = !m_OutputIdx;
	}

	void ParticleEffect::UpdateParticleData(const ParticleComponent& component)
	{
		std::vector<ParticleVertexData> m_VertexData;
		std::vector<ParticleAttribute> m_Attribute;

		if (component.maxParticles > MAX_PARTICLES)
		{
			LOG_ERR("Max number of particles is %d", MAX_PARTICLES);
		}
		else if (component.maxParticles > m_MaxParticles)
		{
			m_MaxParticles = component.maxParticles;

			m_Buffers[1]->m_Buffer[0]->Resize((void*)NULL, m_MaxParticles * sizeof(ParticleAttribute));
			m_Buffers[1]->m_Buffer[1]->Resize((void*)NULL, m_MaxParticles * sizeof(ParticleAttribute));

			LOG_INFO("Resizing particle buffer");
		}

		m_VertexData.resize(m_MaxParticles);
		m_Attribute.resize(m_MaxParticles);

		for (size_t i = 0; i < m_MaxParticles; ++i)
		{
			m_Attribute[i].velocity.x = (float)RandomGenerator::Random(-2, 2);
			m_Attribute[i].velocity.y = (float)RandomGenerator::Random(2, 7);
			m_Attribute[i].velocity.z = (float)RandomGenerator::Random(-2, 2);
			m_Attribute[i].velocity.w = 0;

			
			m_Attribute[i].sizeOverLifeTime = component.sizeOverLifeTime;
			m_Attribute[i].lifeSpan = (float)RandomGenerator::Random(2, 3);
			m_Attribute[i].gravity = 0;


			m_VertexData[i].position = glm::vec4(0, 0, 0, 1);
		}
	
		m_Buffers[1]->m_Buffer[0]->Update((void*)& m_Attribute[0], m_MaxParticles * sizeof(ParticleAttribute), 0);
		m_Buffers[1]->m_Buffer[1]->Update((void*)& m_Attribute[0], m_MaxParticles * sizeof(ParticleAttribute), 0);
	}
	void ParticleEffect::SetRenderCount(unsigned int numParticles)
	{	
		if (numParticles > m_MaxParticles)
			LOG_WARN("Trying to render more particles than buffer contain");
		else
			m_ParticlesRender = numParticles;
	}

	void ParticleEffect::Render(const Ref<Shader>& shader, int drawMod)
	{
		RenderCommand::SubmitElementInstanced(m_VAO[m_OutputIdx], m_ParticlesRender, drawMod);
	}


}