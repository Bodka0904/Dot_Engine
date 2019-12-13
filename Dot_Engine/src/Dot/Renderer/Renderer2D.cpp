#include "stdafx.h"
#include "Renderer2D.h"

#include "Dot/Debug/Log.h"

#include <GL/glew.h>

namespace Dot {
	
	Renderer2D::Renderer2D(unsigned int batchSize)
		: m_BatchSize(batchSize)
	{
		m_VAO = std::make_shared<ArrayBuffer>();
		Ref<VertexBuffer> VBO = std::make_shared<VertexBuffer>((void*)NULL, m_BatchSize * sizeof(QuadVertex), D_DYNAMIC_DRAW);

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
				m_Buffer->position = data[i].m_Vertices[j].position;
				m_Buffer->texCoord = data[i].m_Vertices[j].texCoord;
				m_Buffer++;
			}
			m_NumDataStored++;
		}
	}
	void Renderer2D::PushOffset(const QuadVertex* data, int len,int offsetElements)
	{
		m_VAO->GetVertexBuffer(0)->Update(data,len*sizeof(QuadVertex),offsetElements*sizeof(QuadVertex));
		if (offsetElements > m_NumDataStored)
			m_NumDataStored = offsetElements + len;
		else
			m_NumDataStored += len;
	}
	void Renderer2D::ClearBuffer()
	{
		m_NumDataStored = 0;
		m_VAO->GetVertexBuffer(0)->Bind();
		m_Buffer = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}
	
	
	void Renderer2D::PrepareForRender()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	}
	void Renderer2D::BeginScene(const Ref<Shader> shader, const Ref<OrthoCamera> camera)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", camera->GetViewProjectionMatrix());
	}
	void Renderer2D::Render()
	{	
		m_VAO->Bind();
		glDrawArrays(GL_QUADS, 0,m_NumDataStored*4);
	}
	void Renderer2D::EndScene()
	{
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);
		glEnable(GL_DEPTH_TEST);
	}
}