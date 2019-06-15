#pragma once
#include "../GuiBuffer.h"
#include <glm/glm.hpp>



struct Button 
{
	Button(glm::vec3 Color)
		: color(Color)
	{}
	

	glm::vec3 color;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec4(color,1)),
		GuiVertex(glm::vec2(1,0),glm::vec4(color,1)),
		GuiVertex(glm::vec2(1,1),glm::vec4(color,1)),
		GuiVertex(glm::vec2(0,1),glm::vec4(color,1))
	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};
};


struct CheckBox
{
	CheckBox(glm::vec3 Color)
		:color(glm::vec3(Color))
	{}

	

	glm::vec3 color;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(1,0),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(1,1),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(0,1),glm::vec4(color,0.8))
	};


	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};
};

