#include "stdafx.h"
#include "Mesh.h"
#include "Buffer.h"
#include "Dot/Graphics/ObjLoader.h"

namespace Dot {
	Mesh::Mesh(std::string filename, BufferLayout layout)
	{
	
		std::shared_ptr<VertexBuffer> m_VBO;
		std::shared_ptr<IndexBuffer> m_IBO;


		m_VAO.reset(new Dot::ArrayBuffer);
		Dot::IndexedModel model = Dot::OBJModel(filename).ToIndexedModel();


		m_VBO.reset((new Dot::VertexBuffer((Dot::VertexTexture*) & model.m_vertex[0], model.m_vertex.size() * sizeof(Dot::VertexTexture), Static_Buffer_Update)));
		m_VBO->SetLayout(layout);
		m_VAO->AddVBO(m_VBO);
	

		m_IBO.reset(new Dot::IndexBuffer((unsigned int*)& model.indices[0], model.indices.size()));
		m_VAO->AddIBO(m_IBO);
		
	}


	Mesh::~Mesh()
	{
		
	}

	std::shared_ptr<ArrayBuffer> Mesh::GetVao()
	{
		return m_VAO;	
	}


}
