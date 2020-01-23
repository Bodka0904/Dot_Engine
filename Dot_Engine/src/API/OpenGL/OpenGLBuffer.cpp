#include "stdafx.h"
#include "OpenGLBuffer.h"

#include <GL/glew.h>
namespace Dot {
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size, int drawMod)
		: m_Size(size),m_DrawMod(drawMod)
	{
		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, drawMod);
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_VBO);
	}
	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}
	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenGLVertexBuffer::Update(const void* vertices, unsigned int size, int offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
	}
	
	void* OpenGLVertexBuffer::MapBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}
	void OpenGLVertexBuffer::UnMapBuffer()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenGLVertexBuffer::ClearBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Size, NULL, m_DrawMod);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void OpenGLVertexBuffer::Resize(const void* vertices, unsigned int size)
	{
		m_Size = size;
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, D_DYNAMIC_DRAW);
	}

	void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layoutout = layout;
	}
	
	const BufferLayout& OpenGLVertexBuffer::GetLayout() const
	{
		return m_Layoutout;
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(const void* indices, unsigned int count)
		:m_Count(count)
	{
		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_VBO);
	}
	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}
	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	unsigned int OpenGLIndexBuffer::GetCount() const
	{
		return m_Count;
	}
	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(float* data, unsigned int size, int drawMod)
		:m_Size(size)
	{
		glGenBuffers(1, &m_SSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, drawMod);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
	OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_SSBO);
	}
	void OpenGLShaderStorageBuffer::BindBase(unsigned int point)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, point, m_SSBO);
	}
	void OpenGLShaderStorageBuffer::BindRange(int offset,int size,unsigned int index)
	{
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, m_SSBO, offset, size);
	}
	void OpenGLShaderStorageBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
	}
	void OpenGLShaderStorageBuffer::Update(const void* vertices, unsigned int size, int offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
	}
	void OpenGLShaderStorageBuffer::Resize(const void* vertices, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_SSBO);
		glBufferData(GL_ARRAY_BUFFER, size, vertices,D_DYNAMIC_DRAW);
	}
	void OpenGLShaderStorageBuffer::SetLayout(const BufferLayout& layout)
	{
		m_Layoutout = layout;
	}
	inline const BufferLayout& OpenGLShaderStorageBuffer::GetLayout() const
	{
		return m_Layoutout;
	}
}