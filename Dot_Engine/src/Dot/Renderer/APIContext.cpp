#include "stdafx.h"
#include "APIContext.h"

#include "Dot/Renderer/Renderer.h"
#include "API/OpenGL/OpenGLContext.h"

namespace Dot {

	Scope<APIContext> APIContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}