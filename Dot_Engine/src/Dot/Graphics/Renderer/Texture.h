#pragma once
#include "Mesh.h"
#include "Dot/Debug/Log.h"



struct TextureData
{
	GLuint texture;
	unsigned char* imageData;
	int width;
	int height;
	int numComponents;
};

class Texture
{
public:
	Texture(int numTextures);
	virtual ~Texture();
	
	void Create(std::string fileName);
	void Bind(unsigned int unit);

private:
	TextureData * data;
	unsigned int texIndex = 0;
	
};

