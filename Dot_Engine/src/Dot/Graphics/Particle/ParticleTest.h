#pragma once
#include "ParticleEffect.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>


//Class for testing purpose, need compute shader in order to create particle system
class ParticleTest : public ParticleEffect
{
public:
	ParticleTest(unsigned int num);
	virtual ~ParticleTest();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;
	float RandomFloat();


private:
	std::vector<glm::vec3> offsets;
	std::vector<glm::vec3> velocities;

	std::vector<float> timeAlive;
	std::vector<float> lifeSpan;

	float pSize;
	glm::vec3 color;

	glm::vec3 vertices[1] = {
		glm::vec3(-1, -1,  1)

	};
	unsigned int indices[1] = {
		0 };
	enum
	{
		POSITION_VB,

		COLOR_VB,

		OFFSET_VB,

		INDEX_VB,

		NUM_BUFFERS
	};


	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];

};

