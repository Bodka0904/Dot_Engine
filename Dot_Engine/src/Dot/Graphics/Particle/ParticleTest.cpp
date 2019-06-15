#include "stdafx.h"
#include "ParticleTest.h"



ParticleTest::ParticleTest(unsigned int num)
{
	offsets.resize(num);
	velocities.resize(num);
	timeAlive.resize(num);
	lifeSpan.resize(num);
}


ParticleTest::~ParticleTest()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void ParticleTest::Init()
{

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);


	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), &color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[OFFSET_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * offsets.size(), 0, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(2, 1);


	glBindVertexArray(0);
}

void ParticleTest::Update()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[OFFSET_VB]);
	glBufferData(GL_ARRAY_BUFFER, offsets.size() * sizeof(glm::vec3), &offsets[0], GL_DYNAMIC_DRAW);
}

void ParticleTest::Draw()
{
	glPointSize(10);
	
	

	glBindVertexArray(m_vertexArrayObject);

	glDrawElementsInstanced(GL_POINTS, 1, GL_UNSIGNED_INT, 0, offsets.size());

	glBindVertexArray(0);
}

float ParticleTest::RandomFloat()
{	
	return (float)rand() / ((float)RAND_MAX + 1);
	
}
