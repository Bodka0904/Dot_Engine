#pragma once
#include "Buffer.h"

class ArrayBuffer
{
public:
	ArrayBuffer();
	~ArrayBuffer();

	void Bind() const;
	void UnBind() const;

	void AddVBO(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void AddIBO(const std::shared_ptr<IndexBuffer>& indexBuffer);

private:
	unsigned int m_VAO;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
};

