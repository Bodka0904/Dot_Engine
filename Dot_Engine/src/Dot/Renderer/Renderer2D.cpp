#include "stdafx.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Dot/Debug/Log.h"


namespace Dot {
	
	Renderer2D::Renderer2D(unsigned int batchSize)
		: m_BatchSize(batchSize)
	{
		m_VAO = ArrayBuffer::Create();
		Ref<VertexBuffer> VBO = VertexBuffer::Create((void*)NULL, m_BatchSize * sizeof(QuadVertex), D_DYNAMIC_DRAW);

		BufferLayout layout = {
			{0,ShaderDataType::Float2,"a_Position"},
			{1,ShaderDataType::Float2,"a_TexCoord"}
		};
		m_NumDataStored = 0;
		VBO->SetLayout(layout);
		m_VAO->AddVBO(VBO);

	}
	void Renderer2D::Push(const QuadVertex* data, int len)
	{
		int tmp = m_NumDataStored;
		for (int i = tmp; i < len + tmp; i++)
		{
			for (int j = 0; j < 4; ++j)
			{
				m_Buffer->position = data[i].vertices[j].position;
				m_Buffer->texCoord = data[i].vertices[j].texCoord;
				m_Buffer++;
			}
			m_NumDataStored++;
		}
		m_VAO->GetVertexBuffer(0)->SetCount(m_NumDataStored * 4);
	}
	void Renderer2D::PushOffset(const QuadVertex* data, int len,int offsetElements)
	{
		m_VAO->GetVertexBuffer(0)->Update(data,len*sizeof(QuadVertex),offsetElements*sizeof(QuadVertex));
		if (offsetElements > m_NumDataStored)
			m_NumDataStored = offsetElements + len;
		else
			m_NumDataStored += len;

		m_VAO->GetVertexBuffer(0)->SetCount(m_NumDataStored * 4);
	}

	
	void Renderer2D::ClearBuffer()
	{
		m_VAO->GetVertexBuffer(0)->ClearBuffer();
	}
	void Renderer2D::RestartBuffer()
	{
		m_NumDataStored = 0;
		m_Buffer = (Vertex*)m_VAO->GetVertexBuffer(0)->MapBuffer();
	}
	
	void Renderer2D::PrepareForRender()
	{
		m_VAO->GetVertexBuffer(0)->UnMapBuffer();
	}
	void Renderer2D::BeginScene(const Ref<Shader>& shader, const Ref<OrthoCamera>& camera)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", camera->GetViewProjectionMatrix());

	}
	void Renderer2D::Render()
	{	
		RenderCommand::SubmitArrays(m_VAO, D_QUADS);
	}
	void Renderer2D::EndScene()
	{
	}
}