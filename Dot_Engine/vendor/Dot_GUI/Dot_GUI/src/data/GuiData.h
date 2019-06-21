#pragma once
#include <glm/glm.hpp>
#include "../GuiBuffer.h"


struct Wrapper
{
	static float WRAPPER_SIZE_X;
	static float WRAPPER_SIZE_Y;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(WRAPPER_SIZE_X,0),glm::vec2(1,0)),
		GuiVertex(glm::vec2(WRAPPER_SIZE_X,WRAPPER_SIZE_Y),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,			   WRAPPER_SIZE_Y),glm::vec2(0,1))
	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};

struct Button 
{
	static float BUTTON_SIZE_X;
	static float BUTTON_SIZE_Y;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(BUTTON_SIZE_X,0),glm::vec2(1,0)),
		GuiVertex(glm::vec2(BUTTON_SIZE_X,BUTTON_SIZE_Y),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,            BUTTON_SIZE_Y),glm::vec2(0,1))
	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};


struct CheckBox
{
	static float CHECKBOX_SIZE_X;
	static float CHECKBOX_SIZE_Y;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(CHECKBOX_SIZE_X,0),glm::vec2(1,0)),
		GuiVertex(glm::vec2(CHECKBOX_SIZE_X,CHECKBOX_SIZE_Y),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,		        CHECKBOX_SIZE_Y),glm::vec2(0,1))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};

struct Slider
{	
	
	GuiVertex m_vertices[8] = {
		GuiVertex(glm::vec2(0, 0.4),glm::vec2(0,0)),
		GuiVertex(glm::vec2(4, 0.4),glm::vec2(1,0)),
		GuiVertex(glm::vec2(4, 0.6),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0, 0.6),glm::vec2(0,1)),
	
		GuiVertex(glm::vec2(0,  0.2),glm::vec2(0,0)),
		GuiVertex(glm::vec2(0.4,0.2),glm::vec2(1,0)),
		GuiVertex(glm::vec2(0.4,0.8),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,  0.8),glm::vec2(0,1))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};

struct ArrowButton
{
	static float ARROW_BUTTON_SIZE_X;
	static float ARROW_BUTTON_SIZE_Y;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(ARROW_BUTTON_SIZE_X,0),glm::vec2(1,0)),
		GuiVertex(glm::vec2(ARROW_BUTTON_SIZE_X,ARROW_BUTTON_SIZE_Y),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,		            ARROW_BUTTON_SIZE_Y),glm::vec2(0,1))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};
};