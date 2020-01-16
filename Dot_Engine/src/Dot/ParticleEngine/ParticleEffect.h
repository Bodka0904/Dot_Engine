#pragma once
#include "Dot/Renderer/Renderable.h""
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/Renderer/Camera/Camera.h"
#include "Dot/Renderer/Shader/Shader.h"

namespace Dot {
#define MAX_PARTICLES 10000

	struct Vertex3D
	{
		glm::vec3 position;
		glm::vec2 texcoord;
	};
	struct Quad3D
	{
		Quad3D(const glm::vec3& position, const glm::vec2& size)
		{
			vertex[0].position = glm::vec3(position.x - size.x / 2, position.y - size.y / 2, position.z);
			vertex[1].position = glm::vec3(position.x + size.x / 2, position.y - size.y / 2, position.z);
			vertex[2].position = glm::vec3(position.x + size.x / 2, position.y + size.y / 2, position.z);
			vertex[3].position = glm::vec3(position.x - size.x / 2, position.y + size.y / 2, position.z);

			vertex[0].texcoord = glm::vec2(0,0);
			vertex[1].texcoord = glm::vec2(1,0);
			vertex[2].texcoord = glm::vec2(1,1);
			vertex[3].texcoord = glm::vec2(0,1);

			//vertex[0].texcoord = glm::vec2(1.0f / 8.0f,  1.0f / 8.0f);
			//vertex[1].texcoord = glm::vec2(2.0f / 8.0f,	 1.0f / 8.0f);
			//vertex[2].texcoord = glm::vec2(2.0f / 8.0f,	 2.0f / 8.0f);
			//vertex[3].texcoord = glm::vec2(1.0f / 8.0f,  2.0f / 8.0f);
		}
		Vertex3D vertex[4];
	};
	struct ParticleVertexData
	{	
		// position , transparency
		glm::vec4 position;
	};

	struct ParticleAttribute
	{
		enum
		{
			BIGGER,
			SMALLER,
			NONE
		};
		// velocity, timeAlive
		glm::vec4 velocity;
		int sizeOverLifeTime;
		float lifeSpan;
		float gravity;
		float unused;
	};
	
	struct ParticleBuffer
	{
		ParticleBuffer(unsigned int firstBindPoint, unsigned int secondBindPoint, void* data, unsigned int size, BufferLayout layout = BufferLayout{}, bool vertexBuff = false)
			: m_FirstBindPoint(firstBindPoint), m_SecondBindPoint(secondBindPoint), m_Layout(layout), m_VertexBuffer(vertexBuff)
		{
			m_Buffer[0] = ShaderStorageBuffer::Create(data, size, D_DYNAMIC_DRAW);
			m_Buffer[1] = ShaderStorageBuffer::Create(data, size, D_DYNAMIC_DRAW);
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

	struct ParticleComponent
	{
		std::string computeShader;
		unsigned int maxParticles;
		unsigned int numParticles;
		float speed;
		float gravity;
		float minLifeSpan;
		float maxLifeSpan;
		int   sizeOverLifeTime;
	};

	class ParticleEffect : public Renderable
	{
	public:
		ParticleEffect(const ParticleComponent& component);
		void UpdateParticleData(const ParticleComponent& component);
		void SetRenderCount(unsigned int numParticles);
		void SwapBuffers();		

		virtual void Render(const Ref<Shader>& shader, int drawMod) override;

		const unsigned int GetMaxParticles() const { return m_MaxParticles; }
	private:
		enum
		{
			POSITION,
			VELOCITY,
			NUM_TYPES
		};
		
		Ref<ArrayBuffer> m_VAO[2];
		Ref<ParticleBuffer> m_Buffers[NUM_TYPES];

		int m_OutputIdx = 0;
		unsigned int m_ParticlesRender = 10;
		unsigned int m_MaxParticles = 500;
	};
}