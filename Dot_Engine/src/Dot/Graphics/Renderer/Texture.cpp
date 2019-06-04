#include "stdafx.h"
#include "Texture.h"
#include <cassert>
#include "stb_image.h"


Texture::Texture(int numTextures)
	:
	data(new TextureData[numTextures])
{
}


Texture::~Texture()
{

	glDeleteTextures(texIndex, &data->texture);
}

void Texture::Create(std::string fileName)
{
	data[texIndex].imageData = stbi_load(fileName.c_str(), &data[texIndex].width, &data[texIndex].height, &data[texIndex].numComponents, 4);
	if (data[texIndex].imageData == NULL)
	{
		LOG_ERR("Texture: Could not load texture index: %d",texIndex)
	}
	
	glGenTextures(1, &data[texIndex].texture);
	glBindTexture(GL_TEXTURE_2D, data[texIndex].texture);


	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //reading texture width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //reading texture height

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, data[texIndex].width, data[texIndex].height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[texIndex].imageData);



	glBindTexture(GL_TEXTURE_2D, 0);

	if (data[texIndex].imageData)
	{
		stbi_image_free(data[texIndex].imageData);
	}
	else
	{
		texIndex++;
	}
}

void Texture::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, data[unit].texture);
	 
}
