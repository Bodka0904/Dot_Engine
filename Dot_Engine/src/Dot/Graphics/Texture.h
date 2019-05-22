#pragma once
#include <string>
#include "Mesh.h"
#include <iostream>
#include "Dot/Log.h"



class Texture
{
public:
	Texture(const std::string& fileName);
	virtual ~Texture();
	

	void Bind(unsigned int unit);

private:
	std::string m_fileName;
	GLuint m_texture;
	unsigned char* m_imageData;
	int m_width;
	int m_height;
	int m_numComponents;
	
};

