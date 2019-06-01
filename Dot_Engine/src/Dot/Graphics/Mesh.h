#pragma once
#include "ObjLoader.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <GL/glew.h>


class Mesh
{
public:
	Mesh(const std::string& filename);
	void InitMesh(const IndexedModel & model, std::string filename);

	virtual~Mesh();
	void Draw();
	
	static Mesh* CreateMesh(const std::string& filename) { return new Mesh(filename);}

private:
	enum
	{
		POSITION_VB,

		TEXCOORD_VB,

		NORMAL_VB,

		INDEX_VB,

		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned long long int m_drawCount;

};