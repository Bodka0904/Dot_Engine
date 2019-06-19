#pragma once
#include <glm/glm.hpp>
#include "../GuiBuffer.h"


struct Wrapper
{
	Wrapper(glm::vec3 Color)
		: color(Color)
	{}

	static float WRAPPER_SIZE_X;
	static float WRAPPER_SIZE_Y;

	glm::vec3 color;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec4(color,0.5)),
		GuiVertex(glm::vec2(WRAPPER_SIZE_X,0),glm::vec4(color,0.5)),
		GuiVertex(glm::vec2(WRAPPER_SIZE_X,WRAPPER_SIZE_Y),glm::vec4(color,0.5)),
		GuiVertex(glm::vec2(0,			   WRAPPER_SIZE_Y),glm::vec4(color,0.5))
	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};




struct Button 
{
	Button(glm::vec3 Color)
		: color(Color)
	{}
	static float BUTTON_SIZE_X;
	static float BUTTON_SIZE_Y;

	glm::vec3 color;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(BUTTON_SIZE_X,0),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(BUTTON_SIZE_X,BUTTON_SIZE_Y),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(0,            BUTTON_SIZE_Y),glm::vec4(color,0.8))
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

	static float CHECKBOX_SIZE_X;
	static float CHECKBOX_SIZE_Y;

	glm::vec3 color;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(CHECKBOX_SIZE_X,0),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(CHECKBOX_SIZE_X,CHECKBOX_SIZE_Y),glm::vec4(color,0.8)),
		GuiVertex(glm::vec2(0,		        CHECKBOX_SIZE_Y),glm::vec4(color,0.8))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};


};

struct Slider
{	
	Slider(glm::vec3 Static_Color,glm::vec3 Dynamic_Color)
		: static_color(Static_Color),dynamic_color(Dynamic_Color)
	{}

	
	glm::vec3 static_color;
	glm::vec3 dynamic_color;

	GuiVertex m_vertices[8] = {
		GuiVertex(glm::vec2(0, 0.4),glm::vec4(static_color,0.8)),
		GuiVertex(glm::vec2(4, 0.4),glm::vec4(static_color,0.8)),
		GuiVertex(glm::vec2(4, 0.6),glm::vec4(static_color,0.8)),
		GuiVertex(glm::vec2(0, 0.6),glm::vec4(static_color,0.8)),
	
		GuiVertex(glm::vec2(0,  0.2),glm::vec4(dynamic_color,0.8)),
		GuiVertex(glm::vec2(0.4,0.2),glm::vec4(dynamic_color,0.8)),
		GuiVertex(glm::vec2(0.4,0.8),glm::vec4(dynamic_color,0.8)),
		GuiVertex(glm::vec2(0,  0.8),glm::vec4(dynamic_color,0.8))

	};




	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};
