#pragma once
#include "GuiInfo.h"
#include "Dot/Log.h"


class GuiButton
{
public:
	GuiButton(glm::vec2 size, glm::vec3 color);
	~GuiButton();
	void Init();
	void Update();
	void Draw();
	glm::vec2 &GetTranslation() { return m_translation; }

	void Refresh();

	GuiState &GetState() { return state; }
	GuiData &GetData() { return data ; }


private:
	GuiState state = GuiState::NONE;
	GuiData data;
	glm::vec2 m_translation = glm::vec2(0, 0);

private:
	GLuint m_vao;
	GLuint m_vbo[3];
	const float transparency = 0.7;
};

