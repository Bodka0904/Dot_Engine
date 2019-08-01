#pragma once
#include <glm/glm.hpp>
#include "../graphics/GuiBuffer.h"


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

	Button(const glm::vec2& size)
		: m_size(size)
	{};

	glm::vec2 m_size;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(m_size.x,0),glm::vec2(1,0)),
		GuiVertex(glm::vec2(m_size.x,m_size.y),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,            m_size.y),glm::vec2(0,1))
	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};
struct ArrowButton
{
	ArrowButton(const glm::vec2& size)
		: m_size(size)
	{};

	glm::vec2 m_size;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(m_size.x,0),glm::vec2(1,0)),
		GuiVertex(glm::vec2(m_size.x,m_size.y),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,		            m_size.y),glm::vec2(0,1))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};
};

struct CheckBox
{
	CheckBox(const glm::vec2& size)
		: m_size(size)
	{};

	glm::vec2 m_size;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0,0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(m_size.x,0),glm::vec2(0.5,0)),
		GuiVertex(glm::vec2(m_size.x,m_size.y),glm::vec2(0.5,1)),
		GuiVertex(glm::vec2(0,		        m_size.y),glm::vec2(0,1))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};

struct Slider
{	
	Slider(const glm::vec2& size)
		: m_size(size)
	{};

	glm::vec2 m_size;

	GuiVertex m_vertices[4] = {
		GuiVertex(glm::vec2(0, 0),glm::vec2(0,0)),
		GuiVertex(glm::vec2(m_size.x, 0),glm::vec2(1,0)),
		GuiVertex(glm::vec2(m_size.x, m_size.y),glm::vec2(1,1)),
		GuiVertex(glm::vec2(0,			   m_size.y),glm::vec2(0,1))

	};

	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};

};

