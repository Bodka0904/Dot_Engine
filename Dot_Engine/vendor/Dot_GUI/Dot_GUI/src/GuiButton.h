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

	
	bool MouseHoover(glm::vec2 mousePos);
	bool Clicked() { return m_data->clicked; }

	glm::vec2 &GetTranslation() { return b_translation; }
	glm::vec4 GetCoords() const;


	Button *GetData() const { return m_data; }

private:
	Button *m_data;

	GLuint m_vao;
	GLuint m_vbo[3];
	const float transparency = 0.7;

private:
	glm::vec2 b_translation = glm::vec2(0, 0);

};

