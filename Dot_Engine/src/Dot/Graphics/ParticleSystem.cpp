#include "ParticleSystem.h"



ParticleSystem::ParticleSystem(unsigned int numParticles,float size,glm::vec3 color)
	:
	particles(new Particle[numParticles]),
	pSize(size),
	color(color)
	
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);


	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles[0].offset), &particles[0].offset, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), &color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


	
	
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[OFFSET_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numParticles, 0, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(3, 1);
	
	
	
	glBindVertexArray(0);
}


ParticleSystem::~ParticleSystem()
{
	delete particles;
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}


void ParticleSystem::Update()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[OFFSET_VB]);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(particles) / sizeof(*particles)) * sizeof(glm::vec3), &particles->offset, GL_DYNAMIC_DRAW);
}

void ParticleSystem::Draw()
{
	glPointSize(pSize);
	glBindVertexArray(m_vertexArrayObject);

	glDrawElementsInstanced(GL_POINTS, 1, GL_UNSIGNED_INT, 0, sizeof(particles) / sizeof(*particles));

	glBindVertexArray(0);
}
