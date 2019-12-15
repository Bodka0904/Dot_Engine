#include "stdafx.h"
#include "OpenGLArrayBuffer.h"

#include <GL/glew.h>

namespace Dot {
	OpenGLArrayBuffer::OpenGLArrayBuffer()
	{
		glGenVertexArrays(1, &m_VAO);
	}
	OpenGLArrayBuffer::~OpenGLArrayBuffer()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
	void OpenGLArrayBuffer::Bind() const
	{
		glBindVertexArray(m_VAO);
	}
	void OpenGLArrayBuffer::UnBind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLArrayBuffer::AddVBO(const Ref<VertexBuffer>& vertexBuffer)
	{
		D_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		glBindVertexArray(m_VAO);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();

		for (const auto& element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(element.index);
			glVertexAttribPointer(element.index,
				element.GetComponentCount(),
				GL_FLOAT,
				GL_FALSE,
				layout.GetStride(),
				(const void*)element.offset);

			glVertexAttribDivisor(element.index, element.divisor);

		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLArrayBuffer::AddIBO(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_VAO);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
	void OpenGLArrayBuffer::AddSSBO(const Ref<ShaderStorageBuffer>& shaderSBuffer)
	{
		D_ASSERT(shaderSBuffer->GetLayout().GetElements().size(), "Shader Storage Buffer has no layout");

		glBindVertexArray(m_VAO);
		shaderSBuffer->Bind();

		const auto& layout = shaderSBuffer->GetLayout();

		for (const auto& element : shaderSBuffer->GetLayout())
		{
			glEnableVertexAttribArray(element.index);
			glVertexAttribPointer(element.index,
				element.GetComponentCount(),
				GL_FLOAT,
				GL_FALSE,
				layout.GetStride(),
				(const void*)element.offset);

			glVertexAttribDivisor(element.index, element.divisor);

		}
		m_ShaderBuffers.push_back(shaderSBuffer);
	}
}