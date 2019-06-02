#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>


struct Particle {
	
	glm::vec3 offset;
	glm::vec3 velocity;
	float timeAlive;
	float lifeSpan;

};


class ParticleSystem
{
public:
	ParticleSystem(unsigned int numParticles,float size,glm::vec3 color);
	~ParticleSystem();
	virtual void Init(){};
	void Update();
	void Draw();

	Particle *GetParticles() { return particles; }
		

private:
	Particle *particles;
	float pSize;
	glm::vec3 color;

	enum
	{
		POSITION_VB,

		COLOR_VB,

		OFFSET_VB,

		NUM_BUFFERS
	};


	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];

};

