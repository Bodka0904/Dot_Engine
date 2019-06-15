#pragma once
#include "GuiWidget.h"
#include "../GuiBuffer.h"
#include "../GuiTransform.h"
#include "../glText/GuiText.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>




class GuiButton : public GuiWidget
{
public:

	GuiButton(const std::string& name);
	virtual ~GuiButton();
	virtual void Init(unsigned int& VBO, unsigned int& IBO) override;
	virtual void Draw() override;


	virtual void UpdateData(GuiTransform& transform) override;
	virtual void SetData(glm::vec2 pos, glm::vec2 scale = glm::vec2(0.1, 0.1),glm::vec2 rot = glm::vec2(3.14,0)) override;
	
	virtual bool MouseHoover(glm::vec2 mousePos) override;
	virtual bool &Clicked() override { return clicked; }
	
	
	virtual glm::vec2 GetPosition() const override { return m_position; }
	virtual GuiText *GetText() override { return m_text; }

	

private:
	glm::vec4 GetCoords();

private:
	GLuint m_vao;

private:
	GuiText *m_text;

	glm::vec2 m_position = glm::vec2(0,0);
	glm::vec2 m_scale;
	glm::vec2 m_rotation;


	bool clicked = false;
	const unsigned int indices[6] = {
		0,1,2,0,3,2
	};
};

