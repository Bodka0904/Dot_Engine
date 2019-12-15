#pragma once
#include "Dot/Renderer/Texture.h"

#include <GL/glew.h>
namespace Dot {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& fileName,bool filters = true,bool flipped = true);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(unsigned int unit) override;
	private:
		int CalculateMipMapCount(int width, int height);

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;

		GLenum m_InternalFormat, m_DataFormat;

	};
	

	class OpenGLCubeMapTexture : public CubeMapTexture
	{
	public:
		OpenGLCubeMapTexture(const std::vector<std::string>faces);
		virtual ~OpenGLCubeMapTexture();

		virtual void Bind(unsigned int unit) override;
	private:
		unsigned int texture;
		int textureType;
	};
}