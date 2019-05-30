#pragma once
#include "shaders/GuiShader.h"
#include "GuiInfo.h"
#include "GuiButton.h"




class GuiWindow
{
public:
	GuiWindow(glm::vec2 size,glm::vec3 color);
	~GuiWindow();
	void Init();
	void Draw();

	void AddButton(glm::vec2 size, glm::vec3 color) { buttons.push_back(new GuiButton(size, color)); }

	glm::vec2 &GetTranslation() { return m_translation; }
	GuiData &GetData() {return data;}
	static GuiWindow *CreateWin(glm::vec2 size, glm::vec3 color) { return new GuiWindow(size, color); }

private:
	GuiData data;
	glm::vec2 m_translation = glm::vec2(0,0);

	std::vector<GuiButton*> buttons;
private:
	GLuint m_vao;
	GLuint m_vbo[3];
	const float transparency = 0.4;
	
};

