#include "stdafx.h"
#include "InstancedMesh.h"
#include "Buffer.h"
#include "Dot/Graphics/ObjLoader.h"

namespace Dot {
	InstancedMesh::InstancedMesh(const std::string& filename, BufferLayout layout, std::vector<glm::mat4> matrices)
		:m_num(matrices.size())
	{
		std::shared_ptr<VertexBuffer> m_VBO;
		std::shared_ptr<VertexBuffer> m_VBO_MAT;

		std::shared_ptr<IndexBuffer> m_IBO;

		m_VAO.reset(new Dot::ArrayBuffer);
		Dot::IndexedModel model = Dot::OBJModel(filename).ToIndexedModel();


		BufferLayout mat4 = {
				{3, Dot::ShaderDataType::Mat4, "instanceModel", 1},			
		};
		
		m_VBO.reset((new VertexBuffer((VertexTexture*) & model.m_vertex[0], model.m_vertex.size() * sizeof(Dot::VertexTexture), Static_Buffer_Update)));
		m_VBO->SetLayout(layout);
		m_VAO->AddVBO(m_VBO);
		
		m_VBO_MAT.reset((new VertexBuffer(&matrices[0], m_num * sizeof(glm::mat4), Dynamic_Buffer_Update)));
		m_VBO_MAT->SetLayout(mat4);
		m_VAO->AddVBO(m_VBO_MAT);

		m_IBO.reset(new Dot::IndexBuffer((unsigned int*)& model.indices[0], model.indices.size()));
		m_VAO->AddIBO(m_IBO);

	}
	InstancedMesh::~InstancedMesh()
	{
		
	}
	std::shared_ptr<ArrayBuffer> InstancedMesh::GetVao()
	{
		return m_VAO;
	}
}