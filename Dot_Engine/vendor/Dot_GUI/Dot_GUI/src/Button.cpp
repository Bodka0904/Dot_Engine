#include "stdafx.h"
#include "Button.h"


int Button::NumButtons = 0;

Button::Button()
{
}


Button::~Button()
{
}

void Button::Init()
{
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glGenBuffers(3, VBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particles[0].offset), &particles[0].offset, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), &color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);





	glBindBuffer(GL_ARRAY_BUFFER, VBO[OFFSET_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numParticles, 0, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(3, 1);

}

void Button::Draw()
{
	glGenVertexArrays()
}

void Button::AddButton(ButtonParams * button)
{
	buttons.push_back(button);
	NumButtons++;

}
