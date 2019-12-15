#include "stdafx.h"
#include "Buffer.h"

#include "Dot/Renderer/Renderer.h"
#include "API/OpenGL/OpenGLBuffer.h"


namespace Dot {

	Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, unsigned int size, int drawMod)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size,drawMod);
		}

		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	Ref<IndexBuffer> IndexBuffer::Create(const void* indices, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}



	Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(const void* vertices, unsigned int size, int drawMod)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShaderStorageBuffer>(vertices, size, drawMod);
		}

		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	

}