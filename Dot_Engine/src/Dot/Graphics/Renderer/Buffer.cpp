#include "stdafx.h"
#include "Buffer.h"


VertexBuffer::VertexBuffer(VertexTexture *vertices, unsigned int size)
{
	glCreateBuffers(1,&m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}


VertexBuffer::VertexBuffer(VertexColor * vertices, unsigned int size)
{
	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
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

	
OffsetBuffer::OffsetBuffer(glm::vec3 * offsets,unsigned int size,unsigned int sPos)
{
	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(sPos);
	glVertexAttribPointer(sPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(sPos, 1);
}

OffsetBuffer::~OffsetBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void OffsetBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void OffsetBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
