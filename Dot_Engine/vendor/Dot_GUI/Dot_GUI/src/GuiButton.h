#pragma once
#include "widgets/GuiWidgets.h"
#include "GuiTransform.h"
#include <GL/glew.h>
#include "glText/GuiText.h"
#include <glm/glm.hpp>
#include <string>



class GuiButton
{
public:
	GuiButton(const std::string& name,glm::vec3 color);
	~GuiButton();
	void Init();
	void Draw();
	void UpdateData(GuiTransform& transform);
	void SetData(glm::vec2 pos, glm::vec2 scale = glm::vec2(0.1, 0.1),glm::vec2 rot = glm::vec2(3.14,0));
	
	bool MouseHoover(glm::vec2 mousePos);
	bool Clicked() { return m_data.clicked; }
	
	
	glm::vec2 GetPosition() const { return m_position; }
	Button &GetData() { return m_data; }
	GuiText *GetText() { return m_text; }

	static int &GetAttachedButton() { return attachedButton; }

private:
	glm::vec4 GetCoords();

private:
	Button m_data;
	GLuint m_vao;
	GLuint m_vbo[3];

private:
	GuiText *m_text;

	glm::vec2 m_position = glm::vec2(0,0);
	glm::vec2 m_scale;
	glm::vec2 m_rotation;
private:
	static int attachedButton;

};

