#include "stdafx.h"
#include "GBuffer.h"

#include "Dot/Renderer/Renderer.h"
#include "API/OpenGL/OpenGLGBuffer.h"

namespace Dot {
	Ref<GBuffer> GBuffer::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLGBuffer>(width,height);
		}
	}
}