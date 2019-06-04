#include "stdafx.h"
#include "Buffer.h"


VertexBuffer::VertexBuffer(glm::vec3 * vertices, unsigned int size, unsigned int sPos)
{
	glCreateBuffers(1,&m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(sPos);
	glVertexAttribPointer(sPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count)
	:m_Count(count)
{
	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,count * sizeof(unsigned int),indices,GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::GetCount() const
{
	return m_Count;
}



TextureBuffer::TextureBuffer(glm::vec2 *texCoords, unsigned int size, unsigned int sPos)
{
	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, texCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(sPos);
	glVertexAttribPointer(sPos, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

TextureBuffer::~TextureBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void TextureBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void TextureBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



NormalBuffer::NormalBuffer(glm::vec3 * normals, unsigned int size, unsigned int sPos)
{
	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(sPos);
	glVertexAttribPointer(sPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

NormalBuffer::~NormalBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void NormalBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void NormalBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
	
