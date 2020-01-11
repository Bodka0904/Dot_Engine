#include "stdafx.h"
#include "Shader.h"

#include "Dot/Renderer/Renderer.h"
#include "API/OpenGL/OpenGLShader.h"

namespace Dot {

	Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name,filepath);
		}
	
		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}
