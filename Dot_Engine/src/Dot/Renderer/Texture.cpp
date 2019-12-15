#include "stdafx.h"
#include "Texture.h"

#include "Renderer.h"

#include "API/OpenGL/OpenGLTexture.h"

namespace Dot {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(width, height);
		}
		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path,bool filters, bool flipped)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path,filters,flipped);
		}
		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<CubeMapTexture> CubeMapTexture::Create(const std::vector<std::string> faces)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    D_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLCubeMapTexture>(faces);
		}
		D_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}