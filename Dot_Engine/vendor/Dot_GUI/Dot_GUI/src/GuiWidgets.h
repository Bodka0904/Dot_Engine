#pragma once
#include <glm/glm.hpp>


enum class GuiState
{
	NONE,
	CLICKED,
	ON,
	OFF
	
};

struct Button {

	const float scale = 0.2f;

	glm::vec2 vertices[4]
	{
		glm::vec2(0*scale,0*scale),
		glm::vec2(1*scale,0*scale),
		glm::vec2(1*scale,1*scale),
		glm::vec2(0*scale,1*scale)
	};
	glm::vec4 colors[4]
	{
		glm::vec4(0.3,0.3,0.7,0.5),
		glm::vec4(0.3,0.3,0.7,0.5),
		glm::vec4(0.3,0.3,0.7,0.5),
		glm::vec4(0.3,0.3,0.7,0.5)
	};
	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};