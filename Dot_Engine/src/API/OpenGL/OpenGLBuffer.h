#pragma once
#include "Dot/Renderer/Buffers/Buffer.h"

namespace Dot {
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float *vertices, unsigned int size,int drawMod);
		~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;
		virtual void Update(const void* vertices, unsigned int size, int offset);
		virtual void* MapBuffer();
		virtual void  UnMapBuffer();
		virtual void ClearBuffer();
		virtual void Resize(const void* vertices, unsigned int size);
		virtual void SetLayout(const BufferLayout& layout);

		virtual const BufferLayout& GetLayout() const;
	private:
		int m_DrawMod;
		unsigned int m_Size;
		unsigned int m_VBO;
		BufferLayout m_Layoutout;

	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const void* indices, unsigned int count);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual unsigned int GetCount() const override;
	private:
		unsigned int m_VBO;
		unsigned int m_Count;
	};

	class OpenGLShaderStorageBuffer : public ShaderStorageBuffer
	{
	public:
		OpenGLShaderStorageBuffer(float* data,unsigned int size,int drawMod);
		~OpenGLShaderStorageBuffer();

		virtual void BindBase(unsigned int point);
		virtual void BindRange(int offset,int size,unsigned int index);
		virtual void Bind();
		virtual void Update(const void* vertices, unsigned int size, int offset);
		virtual void Resize(const void* vertices, unsigned int size);

		virtual void SetLayout(const BufferLayout& layout);
		virtual const BufferLayout& GetLayout() const ;

	private:
		unsigned int m_Size;
		unsigned int m_SSBO;
		BufferLayout m_Layoutout;
	};
}