#pragma once
#include "ObjLoader.h"
#include "Transform.h"
#include <glm/glm.hpp>
#include <GL/glew.h>


class Vertex
{
public:
	Vertex() = default;

	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& model = glm::vec3(0, 0, 0))
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;

	}

	inline glm::vec3 * GetPos() { return &pos; }
	inline glm::vec2 * GetTexCoord() { return &texCoord; }
	inline glm::vec3 * GetNormal() { return &normal; }
	inline void SetTexCoord(const glm::vec2& tex) { texCoord = tex; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;

};



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