#pragma once
#include "stdafx.h"
#include <GL/glew.h>
#include <glm/glm.hpp>


enum class GuiState
{
	NONE,
	CLICKED,
	ON,
	OFF
};

struct GuiData  {
	GuiData(glm::vec2 Size,glm::vec3 Color)
		:size(Size),color(Color)
	{};

	glm::vec2 size;
	glm::vec3 color;


	GLfloat vertices[8] =
	{
		 0 ,		  0,
		 1 * size.x , 0,
		 1 * size.x , 1 * size.y,
		 0,			  1 * size.y 

	};
	glm::vec4 colors[4] =
	{
		glm::vec4(color,1),
		glm::vec4(color,1),
		glm::vec4(color,1),
		glm::vec4(color,1)
	};

	unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};

