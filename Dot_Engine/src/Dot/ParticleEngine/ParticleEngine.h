#pragma once
#include "Dot/Renderer/Buffers/ArrayBuffer.h"

#include <glm/glm.hpp>

namespace Dot {
	struct ParticleVertexData
	{
		// position , lifeSpan
		glm::vec4 position;
	};

	struct ParticleAttribute
	{
		// velocity, lifeSpan
		glm::vec4 attribute;
	};

	struct ParticleBuffer
	{
		ParticleBuffer(){}
		ParticleBuffer(unsigned int firstBindPoint, unsigned int secondBindPoint, void* data, unsigned int size, BufferLayout layout = BufferLayout{}, bool vertexBuff = false)
			: m_FirstBindPoint(firstBindPoint), m_SecondBindPoint(secondBindPoint), m_Layout(layout), m_VertexBuffer(vertexBuff)
		{
			m_Buffer[0] = ShaderStorageBuffer::Create(data, size, D_STATIC_DRAW);
			m_Buffer[1] = ShaderStorageBuffer::Create(data, size, D_STATIC_DRAW);
			if (vertexBuff)
			{
				m_Buffer[0]->SetLayout(layout);
				m_Buffer[1]->SetLayout(layout);
			}
		}

		Ref<ShaderStorageBuffer> m_Buffer[2];
		BufferLayout m_Layout;
		unsigned int m_FirstBindPoint;
		unsigned int m_SecondBindPoint;
		bool m_VertexBuffer;

	};

	class ParticleEngine
	{
	public:
		ParticleEngine(unsigned int numParticles,const glm::vec3& emitter);
		void SwapBuffers();

		const unsigned int GetNumParticles() const { return m_NumParticles; }
		const Ref<ArrayBuffer>& GetVAO() { return m_VAO[m_OutputIdx]; }
	private:
		enum
		{
			POSITION,
			VELOCITY,
			NUM_TYPES
		};

		float m_Speed;
		float m_Gravity;
		unsigned int m_NumParticles;
		glm::vec3 m_Emitter;

		Ref<ArrayBuffer> m_VAO[2];
		ParticleBuffer m_Buffers[NUM_TYPES];

		int m_OutputIdx = 0;
	};
	
}