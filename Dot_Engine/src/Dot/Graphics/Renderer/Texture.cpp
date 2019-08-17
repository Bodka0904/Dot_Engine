#include "stdafx.h"
#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <GL/glew.h>

namespace Dot {

	

	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &texture);
	}

	void Texture::Create2D(const std::string& fileName,TextureFormat format)
	{
		int texFormat = 0;
		switch (format)
		{
		case Dot::TextureFormat::None:
			break;
		case Dot::TextureFormat::RGB:
			texFormat = GL_RGB;
		case Dot::TextureFormat::RGBA:
			texFormat = GL_RGBA;
	
		}

		int width;
		int height;
		int numComponents;
		unsigned char* imageData;

		imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);
		if (imageData == NULL)
		{
			printf("Texture: Could not load texture");
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, texFormat, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		

		if (imageData)
		{
			stbi_image_free(imageData);
		}

		textureType = GL_TEXTURE_2D;
	}

	void Texture::CreateCubeMap(const std::vector<std::string> faces)
	{
		

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		textureType = GL_TEXTURE_CUBE_MAP;
	}

	void Texture::Bind(unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(textureType, texture);

	}

	int Texture::CalculateMipMapCount(int width, int height)
	{
		int levels = 1;
		while ((width | height) >> levels) {
			levels++;
		}
		return levels;
	}

}