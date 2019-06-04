#pragma once
#include "Dot/Graphics/ObjLoader.h"
#include "Dot/Graphics/Transform.h"
#include "Buffer.h"
#include <glm/glm.hpp>
#include <GL/glew.h>


class Mesh
{
public:
	Mesh(const std::string& filename);
	~Mesh();
	
	void InitMesh(const IndexedModel & model, std::string filename);
	void Draw();
	
	static Mesh* CreateMesh(const std::string& filename) { return new Mesh(filename);}

private:

	unsigned int m_VAO;
	VertexBuffer *m_VertexB;
	IndexBuffer *m_IndexB;
	NormalBuffer *m_NormalB;
	TextureBuffer *m_TextureB;

};



