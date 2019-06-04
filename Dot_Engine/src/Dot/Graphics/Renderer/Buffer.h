#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>


class VertexBuffer
{
public:
	VertexBuffer(glm::vec3 *vertices, unsigned int size,unsigned int sPos);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;


private:
	unsigned int m_VBO;
};



class IndexBuffer
{
public:
	IndexBuffer(unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	unsigned int GetCount() const;

private:
	unsigned int m_VBO;
	unsigned int m_Count;
};


class TextureBuffer
{
public:
	TextureBuffer(glm::vec2 *texCoords, unsigned int size, unsigned int sPos);
	~TextureBuffer();

	void Bind() const;
	void UnBind() const;


private:
	unsigned int m_VBO;

};


class NormalBuffer
{
public:
	NormalBuffer(glm::vec3 *normals, unsigned int size, unsigned int sPos);
	~NormalBuffer();

	void Bind() const ;
	void UnBind() const;

private:
	unsigned int m_VBO;

};

