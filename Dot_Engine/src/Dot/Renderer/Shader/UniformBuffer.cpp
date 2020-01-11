#include "stdafx.h"
#include "UniformBuffer.h"
#include "Dot/Renderer/Renderer.h"
#include "API/OpenGL/OpenGLUniformBuffer.h"

#include <GL/glew.h>
namespace Dot {
	
	Ref<UniformBuffer> UniformBuffer::Create(const void* data, unsigned int size, unsigned int index)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLUniformBuffer>(data,size,index);
		}

		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}