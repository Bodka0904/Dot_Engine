#pragma once
#include <glm/glm.hpp>



struct Button 
{
	Button(glm::vec3 Color)
		: color(Color)
	{}
	
	bool clicked = false;
	glm::vec3 color;

	glm::vec2 vertices[4]
	{
		glm::vec2(0,0),
		glm::vec2(1,0),
		glm::vec2(1,1),
		glm::vec2(0,1)
	};

	glm::vec4 colors[4]
	{
		glm::vec4(color.x,color.y,color.z,0.5),
		glm::vec4(color.x,color.y,color.z,0.5),
		glm::vec4(color.x,color.y,color.z,0.5),
		glm::vec4(color.x,color.y,color.z,0.5)
	};
	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};
