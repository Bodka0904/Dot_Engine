#pragma once
#include "widgets/GuiWidgets.h"
#include <GL/glew.h>




class GuiButton
{
public:
	GuiButton();
	~GuiButton();
	void Init();
	void Draw();

	
	bool MouseHoover(glm::vec2 mousePos);
	bool Clicked() { return m_data.clicked; }
	
	glm::vec2 &GetTranslation() { return b_translation; }
	glm::vec4 GetCoords() const;

	static int &GetAttachedButton() { return attachedButton; }
	Button &GetData() { return m_data; }

private:
	Button m_data;
	GLuint m_vao;
	GLuint m_vbo[3];
	

private:
	glm::vec2 b_translation = glm::vec2(0, 0);

private:
	static int attachedButton;

};

