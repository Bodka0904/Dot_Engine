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

	m_VertexB = new VertexBuffer((glm::vec3*)&model.positions[0], model.positions.size() * sizeof(model.positions[0]),0);
	m_TextureB = new TextureBuffer((glm::vec2*)&model.texCoords[0], model.positions.size() * sizeof(model.texCoords[0]), 1);
	m_NormalB = new NormalBuffer((glm::vec3*)&model.normals[0], model.normals.size() * sizeof(model.normals[0]), 2);
	m_IndexB = new IndexBuffer((unsigned int*)&model.indices[0],model.indices.size());

	glBindVertexArray(0); //we unbind it;
}