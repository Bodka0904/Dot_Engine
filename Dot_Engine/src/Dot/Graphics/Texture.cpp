#include "Texture.h"
#include <cassert>
#include "stb_image.h"


Texture::Texture(const std::string& fileName)
	:
	m_fileName(fileName),m_texture(0),
	m_width(0),m_height(0),m_numComponents(0),m_imageData(nullptr)
{
	m_imageData = stbi_load(fileName.c_str(), &m_width, &m_height, &m_numComponents, 4);
	if (m_imageData == NULL)
	{
		LOG_ERR("Texture: Could not load texture")
	}
	
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	

	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //reading texture width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //reading texture height
	
	
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageData);
	
	

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_imageData)
	{
		stbi_image_free(m_imageData);
	}
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
