#include "stdafx.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Dot/Debug/Log.h"


namespace Dot {

	Renderer2D::Renderer2D(unsigned int batchSize)
		: m_BatchSize(batchSize)
	{
		m_VAO = ArrayBuffer::Create();
		Ref<VertexBuffer> VBO = VertexBuffer::Create((float*)NULL, m_BatchSize * sizeof(QuadVertex2D), D_DYNAMIC_DRAW);

		BufferLayout layout = {
			{0,ShaderDataType::Float4,"a_Color"},
			{1,ShaderDataType::Float2,"a_Position"},
			{2,ShaderDataType::Float2,"a_TexCoord"}
		};
		m_NumDataStored = 0;
		VBO->SetLayout(layout);
		m_VAO->AddVBO(VBO);

	}
	void Renderer2D::Push(const QuadVertex2D* data, int len)
	{
		int tmp = m_NumDataStored;
		if (m_Buffer)
		{
			for (int i = 0; i < len; i++)
			{
				for (int j = 0; j < 4; ++j)
				{
					m_Buffer->position = data[i].vertices[j].position;
					m_Buffer->texCoord = data[i].vertices[j].texCoord;
					m_Buffer->color = data[i].vertices[j].color;
					m_Buffer++;
				}
				m_NumDataStored++;
			}
		}
	}
	void Renderer2D::PushOffset(const QuadVertex2D* data, int len, int offsetElements)
	{
		m_VAO->GetVertexBuffer(0)->Update(data, len * sizeof(QuadVertex2D), offsetElements * sizeof(QuadVertex2D));
		if (offsetElements > m_NumDataStored)
			m_NumDataStored = offsetElements + len;
		else
			m_NumDataStored += len;
	}




	void Renderer2D::ClearBuffer()
	{
		m_VAO->GetVertexBuffer(0)->ClearBuffer();
	}
	void Renderer2D::ClearBufferOffset(int offset, int size)
	{

	}
	void Renderer2D::RestartBuffer()
	{
		m_NumDataStored = 0;
		m_Buffer = (Vertex2D*)m_VAO->GetVertexBuffer(0)->MapBuffer();
	}

	void Renderer2D::PrepareForRender()
	{
		m_VAO->GetVertexBuffer(0)->UnMapBuffer();
	}
	void Renderer2D::BeginScene()
	{

	}
	void Renderer2D::Render()
	{
		RenderCommand::SubmitArraysCount(m_VAO, m_NumDataStored * 4, D_QUADS);
	}
	void Renderer2D::EndScene()
	{
	}
}