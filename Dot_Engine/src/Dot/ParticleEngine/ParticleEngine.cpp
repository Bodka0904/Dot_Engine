#include "stdafx.h"
#include "ParticleEngine.h"

#include "Dot/Utils/Random/RandomGenerator.h"

namespace Dot {

	ParticleEngine::ParticleEngine(unsigned int numParticles, const glm::vec3& emitter)
		:
		m_NumParticles(numParticles),
		m_Emitter(emitter)
	{
		m_VAO[0] = ArrayBuffer::Create();
		m_VAO[1] = ArrayBuffer::Create();

		std::vector<ParticleVertexData> m_VertexData;
		std::vector<ParticleAttribute> m_Attribute;
		m_VertexData.resize(m_NumParticles);
		m_Attribute.resize(m_NumParticles);

		for (int i = 0; i < m_NumParticles; ++i)
		{
			m_Attribute[i].attribute.x = RandomGenerator::Random(-1, 1);
			m_Attribute[i].attribute.y = RandomGenerator::Random( 2, 5);
			m_Attribute[i].attribute.z = RandomGenerator::Random(-1, 1);

			m_Attribute[i].attribute.w = RandomGenerator::Random(1, 2);

			m_VertexData[i].position = glm::vec4(m_Emitter.x,m_Emitter.y,m_Emitter.z, m_Attribute[i].attribute.w);
		}

		BufferLayout buflayout = {
			{0, ShaderDataType::Float4, "position"}

		};
		
		m_Buffers[0] = ParticleBuffer(0, 2, (void*)& m_VertexData[0], m_VertexData.size() * sizeof(ParticleVertexData),buflayout,true);
		
		m_VAO[0]->AddSSBO(m_Buffers[0].m_Buffer[0]);
		m_VAO[1]->AddSSBO(m_Buffers[0].m_Buffer[1]);
		
		m_Buffers[1] = ParticleBuffer(1, 3, (void*)& m_Attribute[0], m_Attribute.size() * sizeof(ParticleAttribute));

	}
	void ParticleEngine::SwapBuffers()
	{
		m_Buffers[0].m_Buffer[!m_OutputIdx]->BindRange(m_Buffers[0].m_FirstBindPoint);
		m_Buffers[0].m_Buffer[m_OutputIdx]->BindRange(m_Buffers[0].m_SecondBindPoint);

		m_Buffers[1].m_Buffer[!m_OutputIdx]->BindRange(m_Buffers[1].m_FirstBindPoint);
		m_Buffers[1].m_Buffer[m_OutputIdx]->BindRange(m_Buffers[1].m_SecondBindPoint);

		m_OutputIdx = !m_OutputIdx;
	}
	
}