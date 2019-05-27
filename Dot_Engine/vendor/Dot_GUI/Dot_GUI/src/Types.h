#pragma once
#include "stdafx.h"
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


struct ButtonParams
{
	ButtonParams(glm::vec2 Position, std::string Name)
		: position(Position), name(Name)
	{};
	
	std::string name;
	glm::vec2 position;
	
	
	glm::vec2 vertices[4] = {
		glm::vec2((0 + position.x),(0 + position.y)),
		glm::vec2((0 + position.x),(1 + position.y)),
		glm::vec2((1 + position.x),(1 + position.y)),
		glm::vec2((1 + position.x),(0 + position.y))
	};
	glm::vec3 color = glm::vec3(0.5, 0.5, 0.5);

	unsigned int indices[4] = {
		0,1,2,3
	};
	
};


struct WinGuiParams
{
	WinGuiParams(const char* Title,int Width,int Height,glm::vec2 Position)
		: title(Title),width(Width),height(Height),position(Position)
	{}
	const char*title;
	int width;
	int height;
	glm::vec2 position;
};