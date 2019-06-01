#pragma once
#include "stdafx.h"
#include "GuiWidgets.h"
#include <GL/glew.h>
#include "Dot/Log.h"



class GuiButton
{
public:
	GuiButton();
	~GuiButton();
	void Init();
	void Draw();

	void Refresh();
	bool MouseHoover(glm::vec2 mousePos);
	
	glm::vec2 &GetTranslation() { return b_translation; }
	glm::vec4 GetCoords() const;

	GuiState &GetState() { return b_state; }


private:
	Button *m_data;

	GLuint m_vao;
	GLuint m_vbo[3];
	const float transparency = 0.7;

private:
	GuiState b_state = GuiState::NONE;
	glm::vec2 b_translation = glm::vec2(0, 0);

};

