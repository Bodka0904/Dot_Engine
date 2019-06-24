#pragma once
#include "Dot/Graphics/ObjLoader.h"
#include "Buffer.h"
#include "ArrayBuffer.h"
#include <glm/glm.hpp>



class Mesh
{
public:
	Mesh(const std::string& filename);
	~Mesh();
	
	void InitMesh(const IndexedModel & model, std::string filename);
	void Draw();
	
	static Mesh* CreateMesh(const std::string& filename) { return new Mesh(filename);}

private:
	ArrayBuffer *m_Renderer;
	VertexBuffer *m_VertexB;
	IndexBuffer *m_IndexB;

};



