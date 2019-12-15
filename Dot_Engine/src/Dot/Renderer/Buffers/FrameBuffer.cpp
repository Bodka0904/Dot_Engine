#include "stdafx.h"
#include "FrameBuffer.h"

#include "Dot/Renderer/Renderer.h"
#include "API/OpenGL/OpenGLFrameBuffer.h"

namespace Dot {
	Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height, FramebufferFormat format)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(width,height,format);
		}
	}
}