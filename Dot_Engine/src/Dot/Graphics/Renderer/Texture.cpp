#include "stdafx.h"
#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <GL/glew.h>

namespace Dot {



	Texture::Texture(const std::string& fileName)
	{
		int texFormat = 0;
		int dataFormat = 0;

		int width;
		int height;
		int numComponents;
		unsigned char* imageData;

		stbi_set_flip_vertically_on_load(1);
		imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);
		if (imageData == NULL)
		{
			printf("Texture: Could not load texture");
		}



		if (numComponents == 4)
		{
			texFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (numComponents == 3)
		{
			texFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		D_ASSERT(texFormat & dataFormat, "Format not supported!");

		glGenTextures(1, &texture);
		glTextureStorage2D(texture, 1, texFormat, width, height);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);


		if (imageData)
		{
			stbi_image_free(imageData);
		}

		textureType = GL_TEXTURE_2D;
	}

	Texture::Texture(const std::vector<std::string> faces)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		stbi_set_flip_vertically_on_load(0);
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

	Texture::~Texture()
	{
		glDeleteTextures(1, &texture);
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

	CubeMapTexture::CubeMapTexture(const std::vector<std::string> faces)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		stbi_set_flip_vertically_on_load(0);
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

	CubeMapTexture::~CubeMapTexture()
	{
		glDeleteTextures(1, &texture);
	}

	void CubeMapTexture::Bind(unsigned int unit)
	{
		assert(unit >= 0 && unit <= 31);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(textureType, texture);
	}

}