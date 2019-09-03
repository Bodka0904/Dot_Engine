#pragma once
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/Graphics/Kernel/Particle/ParticleKernel.cuh"


#ifdef D_CUDA
namespace Dot {
	class ParticleEngine
	{
	public:
		ParticleEngine(unsigned int count);
		~ParticleEngine();

		void Update(float dt);
		
		const unsigned int& GetCount() const { return m_count; }
		const Ref<ArrayBuffer> GetVao() const { return m_VAO; }

	private:
		Ref<ArrayBuffer> m_VAO;
		Ref<DefaultParticleEffect> m_effect;
		Ref<TestParticleEffect> TEST;
		unsigned int m_count;

	};
}

#endif