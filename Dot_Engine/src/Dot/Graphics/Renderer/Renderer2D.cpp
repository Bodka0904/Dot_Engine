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
		
		VBO->SetLayout(layout);
		m_VAO->AddVBO(VBO);
	}
	void Renderer2D::Push(const Ref<Renderable2D> data)
	{
		m_Buffer->position = data->GetQuad().m_Vertices[0].position;
		m_Buffer->texCoord = data->GetQuad().m_Vertices[0].texCoord;
		m_Buffer++;

		m_Buffer->position = data->GetQuad().m_Vertices[1].position;
		m_Buffer->texCoord = data->GetQuad().m_Vertices[1].texCoord;
		m_Buffer++;

		m_Buffer->position = data->GetQuad().m_Vertices[2].position;
		m_Buffer->texCoord = data->GetQuad().m_Vertices[2].texCoord;
		m_Buffer++;

		m_Buffer->position = data->GetQuad().m_Vertices[3].position;
		m_Buffer->texCoord = data->GetQuad().m_Vertices[3].texCoord;
		m_Buffer++;

		m_NumDataStored += 4;
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
		glDrawArrays(GL_QUADS, 0, m_NumDataStored);
	}
	void Renderer2D::EndScene()
	{
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);
		glEnable(GL_DEPTH_TEST);
	}
}