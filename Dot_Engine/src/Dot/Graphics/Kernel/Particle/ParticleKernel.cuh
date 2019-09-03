#pragma once

#ifdef D_CUDA

#include <cuda.h>
#include <curand.h>
#include <curand_kernel.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>



namespace Dot {

	class ParticleEffect
	{
	public:
		ParticleEffect() {}
		ParticleEffect(unsigned int count) {};
		virtual ~ParticleEffect() = default;

		virtual void Launch(float dt) = 0;

	};



	class DefaultParticleEffect : public ParticleEffect
	{
	public:
		DefaultParticleEffect(unsigned int count);
		virtual ~DefaultParticleEffect();


		virtual void Launch(float dt) override;

	public:
		unsigned int m_count;
		float3* m_positions;
		float3* m_velocities;
	};


	class TestParticleEffect
	{
	public:
		TestParticleEffect(unsigned int count);
		~TestParticleEffect();


		void Render(float dt);
		void Launch(float dt);


	private:
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_count;

		void* d_vbo_buffer = NULL;
		float3* m_positions;
		float3* m_velocities;
	};
	
}

#endif