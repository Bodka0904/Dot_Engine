#include "GuiBuffer.h"



GuiVertexBuffer::GuiVertexBuffer(GuiVertex *vertices, unsigned int size)
{
	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

}


GuiVertexBuffer::~GuiVertexBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void GuiVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void GuiVertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GuiIndexBuffer::GuiIndexBuffer(unsigned int * indices, unsigned int count)
{
	glCreateBuffers(1, &m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

}

GuiIndexBuffer::~GuiIndexBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void GuiIndexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void GuiIndexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int GuiIndexBuffer::GetCount() const
{
	return m_Count;
}
