#include "stdafx.h"
#include "ParticleSystem.h"

#include <GL/glew.h>
#include <random>

namespace Dot {



	Dot::ParticleSystem::ParticleSystem(ParticleLayout layout)
		: m_OutputIdx(1)
	{
		m_Count = layout.m_count;
	
		m_VAO[0] = ArrayBuffer::Create();
		m_VAO[1] = ArrayBuffer::Create();

		m_Buffers.resize(layout.buffers.size());
		m_PairedBuffers.resize(layout.pairedBuffers.size());

		for (int i = 0; i < layout.buffers.size(); ++i)
		{
		
			m_Buffers[i].m_Buffer = layout.buffers[i].m_Buffer;
			m_Buffers[i].m_BindPoint = layout.buffers[i].m_BindPoint;	

			if (layout.buffers[i].m_VertexBuffer)
			{			
				m_Buffers[i].m_Buffer->SetLayout(layout.buffers[i].m_Layout);

				m_VAO[0]->AddSSBO(m_Buffers[i].m_Buffer);
				m_VAO[1]->AddSSBO(m_Buffers[i].m_Buffer);
			}

			
		}
		
		for (int i = 0; i < layout.pairedBuffers.size(); ++i)
		{
			
			m_PairedBuffers[i] = new StorageBuffers[2];
		
			m_PairedBuffers[i][0].m_Buffer = layout.pairedBuffers[i].m_Buffer[0];
			m_PairedBuffers[i][1].m_Buffer = layout.pairedBuffers[i].m_Buffer[1];
			
			m_PairedBuffers[i][0].m_BindPoint = layout.pairedBuffers[i].m_FirstBindPoint;
			m_PairedBuffers[i][1].m_BindPoint = layout.pairedBuffers[i].m_SecondBindPoint;

		
			
			if (layout.pairedBuffers[i].m_VertexBuffer)
			{
				
				m_PairedBuffers[i][0].m_Buffer->SetLayout(layout.pairedBuffers[i].m_Layout);
				m_PairedBuffers[i][1].m_Buffer->SetLayout(layout.pairedBuffers[i].m_Layout);
					
				m_VAO[0]->AddSSBO(m_PairedBuffers[i][0].m_Buffer);	
				m_VAO[1]->AddSSBO(m_PairedBuffers[i][1].m_Buffer);		
				
			}
		}

	}

	ParticleSystem::~ParticleSystem()
	{

		for (int i = 0; i < m_PairedBuffers.size(); ++i)
		{
			delete[]m_PairedBuffers[i];
		}
	}

	void ParticleSystem::Update(float dt)
	{
		
		for (int i = 0; i < m_Buffers.size(); ++i)
		{
			m_Buffers[i].m_Buffer->BindRange(m_Buffers[i].m_BindPoint);
		}

		for (int i = 0; i < m_PairedBuffers.size(); i++)
		{
			m_PairedBuffers[i][!m_OutputIdx].m_Buffer->BindRange(m_PairedBuffers[i][0].m_BindPoint);
			m_PairedBuffers[i][m_OutputIdx].m_Buffer->BindRange(m_PairedBuffers[i][1].m_BindPoint);

		}
		m_OutputIdx = !m_OutputIdx;		
	
	}

	void ParticleSystem::Draw()
	{
		m_VAO[m_OutputIdx]->Bind();
		glPointSize(20);
		glDrawArrays(GL_POINTS, 0, m_Count);
	}


}