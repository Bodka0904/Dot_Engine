#include "stdafx.h"
#include "ParticleMesh.h"
#include "Dot/Renderer/RenderCommand.h"

namespace Dot {
	ParticleMesh::ParticleMesh()
		: m_Count(0)
	{
		m_VAO = ArrayBuffer::Create();

		Quad3D quad(glm::vec3(0, 0, 0), glm::vec2(1, 1));
		BufferLayout vbolayout = {
			{1,ShaderDataType::Float3,"position"},
			{2,ShaderDataType::Float2,"texcoord"}
		};
		Ref<VertexBuffer> VBO = VertexBuffer::Create((float*)& quad.vertex[0], sizeof(Vertex3D) * 4, D_DYNAMIC_DRAW);
		VBO->SetLayout(vbolayout);
		m_VAO->AddVBO(VBO);

		unsigned int indices[6] = {
			0,1,2,0,2,3
		};
		Ref<IndexBuffer> IBO = IndexBuffer::Create(&indices[0], 6);
		m_VAO->AddIBO(IBO);
	}
	Dot::ParticleMesh::ParticleMesh(const Ref<ArrayBuffer> vao)
		: m_Count(0)
	{
		m_VAO = vao;
	}

	void Dot::ParticleMesh::Render(const Ref<Shader>& shader, int drawMod)
	{
		RenderCommand::SubmitElementInstanced(m_VAO, m_Count, drawMod);
	}
}