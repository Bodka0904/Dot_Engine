#pragma once
#include "../data/GuiData.h"
#include "../GuiShader.h"
#include "../GuiBuffer.h"
#include "../GuiTransform.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class GuiWrapper
{
public:
	GuiWrapper();
	~GuiWrapper();

	void Init(unsigned int& VBO, unsigned int& IBO);
	void Draw(GuiShader& shader, GuiTransform& transform);
	void UpdateData(GuiTransform& transform);

	void SetWidgetIndex(glm::vec2 rangeID) { m_widgetID = rangeID; }
	void SetData(glm::vec2 pos, glm::vec2 scale = glm::vec2(0.5, 1));
	void SetSize(glm::vec2 scale) { m_scale = scale; };
	void PinToSide(glm::vec2 winSize);
	
	
	float &GetColor() { return m_color; }
	bool &GetPinned() { return m_pinned; }
	bool MouseHoover(glm::vec2 mousePos);
	

	glm::vec2 GetCenter() const;
	glm::vec2 GetPosition() const;
	glm::vec2 GetScale() const { return m_scale; }
	glm::vec2 GetWidgetIndex() const { return m_widgetID; }
	

private:
	glm::vec4 GetCoords();

private:
	GLuint m_vao;
	
	glm::vec2 m_position = glm::vec2(0, 0);
	glm::vec2 m_scale;

	float m_color = 0;

	bool m_pinned = false;

	unsigned int m_width;
	unsigned int m_height;

private:
	glm::vec2 m_widgetID;
};

