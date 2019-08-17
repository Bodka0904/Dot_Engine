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

		void AddVBO(const Ref<VertexBuffer>& vertexBuffer);
		void AddIBO(const Ref<IndexBuffer>& indexBuffer);

		Ref<VertexBuffer> GetVertexBuffer(int index) const { return m_VertexBuffers[index]; }
		Ref<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		unsigned int m_VAO;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}
