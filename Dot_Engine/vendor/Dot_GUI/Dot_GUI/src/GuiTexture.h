#pragma once
#include "GL/glew.h"
#include <string>

struct GuiTextureData
{
	GLuint texture;
	unsigned char* imageData;
	int width;
	int height;
	int numComponents;
};

class GuiTexture
{
public:
	GuiTexture();
	~GuiTexture();

	void Create(std::string fileName);
	void Bind(unsigned int unit);

private:
	GuiTextureData data;
};

