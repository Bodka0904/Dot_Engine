#include "stdafx.h"
#include "Mesh.h"
#include <GL/glew.h>

Mesh::Mesh(const std::string & filename)
{

	IndexedModel model = OBJModel(filename).ToIndexedModel();
	InitMesh(model, filename);

}


Mesh::~Mesh()
{
	delete m_VertexB;
	delete m_IndexB;
	delete m_Renderer;	
}

void Mesh::Draw()
{
	m_Renderer->Bind();

	glDrawElements(GL_TRIANGLES, m_IndexB->GetCount(), GL_UNSIGNED_INT, 0);

	m_Renderer->UnBind();

}



void Mesh::InitMesh(const IndexedModel & model, std::string filename)
{
	m_Renderer = new ArrayBuffer();

	BufferLayout layout = {
				{ ShaderDataType::Float3, "position" },
				{ ShaderDataType::Float3, "normal" },
				{ ShaderDataType::Float2, "texCoord" },
				
	};
	
	m_VertexB = new VertexBuffer((VertexTexture*)&model.m_vertex[0], model.m_vertex.size() * sizeof(VertexTexture));
	m_VertexB->SetLayout(layout);
	m_Renderer->AddVBO((std::shared_ptr<VertexBuffer>)m_VertexB);


	m_IndexB = new IndexBuffer((unsigned int*)&model.indices[0], model.indices.size());	
	m_Renderer->AddIBO((std::shared_ptr<IndexBuffer>)m_IndexB);

}