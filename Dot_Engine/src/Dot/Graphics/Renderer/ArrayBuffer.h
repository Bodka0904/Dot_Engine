#pragma once
#include "Buffer.h"


namespace Dot {

	class ArrayBuffer
	{
	public:
		ArrayBuffer();
		~ArrayBuffer();

		void Bind() const;
		void UnBind() const;

		void AddVBO(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void AddIBO(const std::shared_ptr<IndexBuffer>& indexBuffer);

		std::shared_ptr<VertexBuffer> GetVertexBuffer(int index) const { return m_VertexBuffers[index]; }
		std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		unsigned int m_VAO;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
