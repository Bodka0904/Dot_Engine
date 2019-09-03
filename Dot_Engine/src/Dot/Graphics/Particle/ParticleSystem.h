#pragma once
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"

namespace Dot {

	

	struct ParticlePosition
	{
		float x;
		float y;
		float z;
		float w;         
	};
	struct ParticleVelocity
	{
		float vx;
		float vy;
		float vz;
		float fTimeToLive;  
	};
	struct ParticleRotation
	{
		float x;
		float y;
		float z;
		float w = 0;
	};

	
	struct ParticleBuffer
	{
		template<typename T>
		ParticleBuffer(const std::string& name, unsigned int bindPoint, T data, unsigned int size, BufferLayout layout = BufferLayout{}, bool vertexBuff = false)
			: m_BindPoint(bindPoint),m_VertexBuffer(vertexBuff),m_Layout(layout)
		{
			m_Buffer = std::make_shared<ShaderStorageBuffer>((void*)& data[0], size, D_STATIC_DRAW);
		}

		Ref<ShaderStorageBuffer> m_Buffer;
		BufferLayout m_Layout;
		unsigned int m_BindPoint;
		bool m_VertexBuffer;
	};

	struct PairedParticleBuffers
	{
	
		PairedParticleBuffers(const std::string& name, unsigned int firstBindPoint, unsigned int secondBindPoint,void* data, unsigned int size, BufferLayout layout = BufferLayout{}, bool vertexBuff = false)
			: m_FirstBindPoint(firstBindPoint),m_SecondBindPoint(secondBindPoint), m_Layout(layout),m_VertexBuffer(vertexBuff)
		{

			m_Buffer[0] = std::make_shared<ShaderStorageBuffer>(data, size, D_STATIC_DRAW);
			m_Buffer[1] = std::make_shared<ShaderStorageBuffer>(data, size, D_STATIC_DRAW);
		}
		Ref<ShaderStorageBuffer> m_Buffer[2];
		BufferLayout m_Layout;
		unsigned int m_FirstBindPoint;
		unsigned int m_SecondBindPoint;
		bool m_VertexBuffer;

	};

	struct ParticleLayout
	{	
		ParticleLayout(unsigned int count,std::initializer_list<ParticleBuffer> Buffers,std::initializer_list<PairedParticleBuffers> PairedBuffers)
			:m_count(count),buffers(Buffers),pairedBuffers(PairedBuffers)
		{}
	
		unsigned int m_count;
		std::vector<ParticleBuffer> buffers;
		std::vector<PairedParticleBuffers> pairedBuffers;
	};

	class ParticleEffect
	{
	public:
		virtual void Update(float dt) = 0;
		virtual void Draw() = 0;

	};

	
	class ParticleSystem
	{
	public:
		ParticleSystem(ParticleLayout layout);
		~ParticleSystem();

		void Update(float dt);
		void Draw();

	private:
		int	m_OutputIdx;
		unsigned int m_Count;
		Ref<ArrayBuffer> m_VAO[2];

		struct StorageBuffers
		{
			Ref<ShaderStorageBuffer> m_Buffer;
			unsigned int m_BindPoint = 0;
		};


		std::vector<StorageBuffers> m_Buffers;
		std::vector<StorageBuffers*> m_PairedBuffers;
	};

	
}