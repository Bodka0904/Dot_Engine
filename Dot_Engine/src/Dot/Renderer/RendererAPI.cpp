#include "stdafx.h"
#include "RendererAPI.h"
#include "Api/OpenGL/OpenGLRendererAPI.h"

namespace Dot {
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLRendererAPI>();
		}

		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}