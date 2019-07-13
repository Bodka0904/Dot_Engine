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
		GuiVertex(glm::vec2(WRAPPER_SIZE_X,WRAPPER_SIZE_Y),glm::vec2(1,0.9)),
		GuiVertex(glm::vec2(0,			   WRAPPER_SIZE_Y),glm::vec2(0,0.9))
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

struct CheckBox
{
	static float CHECKBOX_SIZE_X;
	static float CHECKBOX_SIZE_Y;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(CHECKBOX_SIZE_X,0),glm::vec2(0.5,0)),
		GuiVertex(glm::vec2(CHECKBOX_SIZE_X,CHECKBOX_SIZE_Y),glm::vec2(0.5,1)),
		GuiVertex(glm::vec2(0,		        CHECKBOX_SIZE_Y),glm::vec2(0,1))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};

struct Slider
{	
	static float SLIDER_SIZE_X;
	static float SLIDER_SIZE_Y;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0, 0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(SLIDER_SIZE_X, 0),glm::vec2(1,0)),
		GuiVertex(glm::vec2(SLIDER_SIZE_X, SLIDER_SIZE_Y),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,			   SLIDER_SIZE_Y),glm::vec2(0,1))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};

