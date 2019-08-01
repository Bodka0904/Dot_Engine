#include "GuiTexture.h"
#include "stb_image.h"
#include <assert.h>


GuiTexture::GuiTexture()
{
}


GuiTexture::~GuiTexture()
{	
	glDeleteTextures(0, &data.texture);
}

void GuiTexture::Create(std::string fileName)
{
	data.imageData = stbi_load(fileName.c_str(), &data.width, &data.height, &data.numComponents, 4);
	if (data.imageData == NULL)
	{
		printf("GuiTexture: Could not load texture");
	}

	glGenTextures(1, &data.texture);
		
	glBindTexture(GL_TEXTURE_2D, data.texture);

	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //reading texture width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); //reading texture height
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.imageData);
	;

	glBindTexture(GL_TEXTURE_2D, 0);

	if (data.imageData)
	{
		stbi_image_free(data.imageData);
	}
		
	
}

void GuiTexture::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, data.texture);
}
