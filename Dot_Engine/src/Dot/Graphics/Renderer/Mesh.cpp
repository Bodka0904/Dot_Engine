#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(const std::string & filename)
{

	IndexedModel model = OBJModel(filename).ToIndexedModel();

	InitMesh(model, filename);
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);

	delete m_VertexB;
	delete m_IndexB;
	
}

void Mesh::Draw()
{
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_IndexB->GetCount(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}



void Mesh::InitMesh(const IndexedModel & model, std::string filename)
{
	
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	
	BufferLayout layout = {
				{ ShaderDataType::Float3, "position" },
				{ ShaderDataType::Float3, "normal" },
				{ ShaderDataType::Float2, "texCoord" },
				
	};
	
	
	
	m_VertexB = new VertexBuffer((VertexTexture*)&model.m_vertex[0], model.m_vertex.size() * sizeof(VertexTexture));
	m_VertexB->SetLayout(layout);

	
	uint32_t index = 0;
	for (const auto& element : m_VertexB->GetLayout())
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			element.GetComponentCount(),
			GL_FLOAT,
			GL_FALSE,
			layout.GetStride(),
			(const void*)element.offset);
		index++;
	}
	m_IndexB = new IndexBuffer((unsigned int*)&model.indices[0], model.indices.size());

}