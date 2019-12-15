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


void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
	return Ref<Shader>();
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
{
	return Ref<Shader>();
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
	return Ref<Shader>();
}

bool ShaderLibrary::Exists(const std::string& name) const
{
	return false;
}

}
