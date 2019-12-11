
#ifdef D_CUDA

#include "ParticleKernel.cuh"
#include "Dot/Core.h"
#include <GL/glew.h>
#include <cuda_gl_interop.h>
#include <iostream>




namespace Dot {

	__global__ void _Init(float3* pos, float3* vel, int count,curandState * state)
	{
		int id = blockDim.x * blockIdx.x + threadIdx.x;
		

		if (id < count)
		{	
			pos[id] = make_float3(0, 0, 0);

			curand_init(1234, id, 0, &state[id]);
			float rand = curand_uniform(&(state[id]));
			vel[id].x = rand/5;

		
			float rand1 = curand_uniform(&(state[id]));
			vel[id].y = -rand1/5;

		
			float rand2 = curand_uniform(&(state[id]));
			vel[id].z = rand2/5;
		}
	}
	__global__ void _UpdateKernel(float3* pos, float3* vel, int count,float dt)
	{
		int id = blockDim.x * blockIdx.x + threadIdx.x;
		
		if (id < count)
		{
			pos[id].x += vel[id].x * dt;
			pos[id].y += vel[id].y * dt;
			pos[id].z += vel[id].z * dt;

			if (abs(pos[id].x) >= 30 || abs(pos[id].y) >= 30 || abs(pos[id].z) >= 30)
			{
				pos[id].x = 0;
				pos[id].y = 0;
				pos[id].z = 0;
			}
		}
	}

	DefaultParticleEffect::DefaultParticleEffect(unsigned int count)
		: m_count(count), m_positions(NULL), m_velocities(NULL)
	{
		CudaErrChk(cudaMallocManaged(&m_positions, sizeof(float3) * m_count));
		CudaErrChk(cudaMallocManaged(&m_velocities, sizeof(float3) * m_count));

		curandState* d_state;
		CudaErrChk(cudaMalloc(&d_state, sizeof(curandState)));

		_Init << <m_count / 256 + 1,256 >> > (m_positions, m_velocities, m_count,d_state);
		
		CudaErrChk(cudaDeviceSynchronize());
		CudaErrChk(cudaFree(d_state));
		
	}

	DefaultParticleEffect::~DefaultParticleEffect()
	{
		CudaErrChk(cudaDeviceSynchronize());
		CudaErrChk(cudaFree(m_positions));
		CudaErrChk(cudaFree(m_velocities));

		cudaDeviceReset();

	}

	void  DefaultParticleEffect::Launch(float dt)
	{			
		_UpdateKernel << <m_count / 256 + 1, 256 >> > (m_positions, m_velocities, m_count, dt);	
		CudaErrChk(cudaDeviceSynchronize());
	}

	__global__ void _TestInit(float3* pos,float3* vel, int count, curandState* state)
	{
		int id = blockDim.x * blockIdx.x + threadIdx.x;

		if (id < count)
		{
			curand_init(1234, id, 0, &state[id]);
			pos[id] = make_float3(0,0,0);
			
			float rand = curand_uniform(&(state[id]));
			vel[id].x = rand / 5;

		
			float rand1 = curand_uniform(&(state[id]));
			vel[id].y = -rand1 / 5;

	
			float rand2 = curand_uniform(&(state[id]));
			vel[id].z = rand2 / 5;
		}
	}

	__global__ void _TestUpdateKernel(float3* pos, float3* vel, int count, float dt)
	{
		int id = blockDim.x * blockIdx.x + threadIdx.x;

		if (id < count)
		{
			pos[id].x += vel[id].x * dt;
			pos[id].y += vel[id].y * dt;
			pos[id].z += vel[id].z * dt;
		}
	}

	TestParticleEffect::TestParticleEffect(unsigned int count)
	: m_count(count)
	{
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(float3), NULL, GL_DYNAMIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(0, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		

		CudaErrChk(cudaGLRegisterBufferObject(m_VBO));

		
		curandState* d_state;

		cudaMalloc(&m_velocities, m_count * sizeof(float3));
		CudaErrChk(cudaMalloc(&d_state, sizeof(curandState)));
		CudaErrChk(cudaMalloc(&m_positions, m_count * sizeof(float3)));

		CudaErrChk(cudaGLMapBufferObject((void**)& m_positions, m_VBO));

		_TestInit << <m_count / 256 + 1, 256 >> > (m_positions,m_velocities, m_count,d_state);

		CudaErrChk(cudaGLUnmapBufferObject(m_VBO));

		cudaFree(d_state);

		glBindVertexArray(0);
		cudaDeviceSynchronize();
	}

	TestParticleEffect::~TestParticleEffect()
	{
		CudaErrChk(cudaDeviceSynchronize());
		CudaErrChk(cudaGLUnregisterBufferObject(m_VBO));

		CudaErrChk(cudaFree(m_positions));
		CudaErrChk(cudaFree(m_velocities));

		glDeleteBuffers(1, &m_VBO);
		
		glDeleteBuffers(1, &m_VAO);
	}

	void TestParticleEffect::Render(float dt)
	{

		cudaDeviceSynchronize();
	
		CudaErrChk(cudaGLMapBufferObject((void**)& m_positions, m_VBO));
		
		_TestUpdateKernel << <m_count/256+1, 256 >> > (m_positions,m_velocities, m_count, dt);
	
		CudaErrChk(cudaGLUnmapBufferObject(m_VBO));

		cudaDeviceSynchronize();

		
		glBindVertexArray(m_VAO);

		glDrawArraysInstanced(GL_POINTS, 0,1,m_count);

		glBindVertexArray(0);

	}

	void TestParticleEffect::Launch(float dt)
	{
	}

}


#endif