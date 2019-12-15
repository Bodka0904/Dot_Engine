#include "stdafx.h"
#include "ArrayBuffer.h"

#include "Dot/Renderer/Renderer.h"
#include "API/OpenGL/OpenGLArrayBuffer.h"


namespace Dot {

	Ref<ArrayBuffer> ArrayBuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLArrayBuffer>();
		}

		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}



}