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
	void SetData(glm::vec2 pos, glm::vec2 scale = glm::vec2(1, 1));
	void SetSize(glm::vec2 scale) { m_scale = scale; };

	float &GetColor() { return m_color; }

	bool Resize(glm::vec2 mousePos);
	bool MouseHoover(glm::vec2 mousePos) ;
	bool &Clicked() { return clicked; };

	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetWidgetIndex() const { return m_widgetID; }


private:
	glm::vec4 GetCoords();

private:
	GLuint m_vao;
	
	glm::vec2 m_position = glm::vec2(0, 0);
	glm::vec2 m_scale;

	float m_color = 0;


	bool clicked = false;

	unsigned int m_width;
	unsigned int m_height;

private:
	glm::vec2 m_widgetID;
};

