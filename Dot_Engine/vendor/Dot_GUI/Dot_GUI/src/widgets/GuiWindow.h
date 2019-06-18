#pragma once
#include "GuiWidget.h"
#include "../GuiShader.h"
#include "../GuiBuffer.h"
#include "../GuiTransform.h"
#include "../glText/GuiText.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class GuiWindow : public GuiWidget
{
public:
	GuiWindow(const std::string& name);
	virtual ~GuiWindow();

	virtual void Init(unsigned int& VBO, unsigned int& IBO) override;
	virtual void Draw(GuiShader& shader, GuiTransform& transform) override;
	virtual void UpdateData(GuiTransform& transform) override;

	virtual void SetData(glm::vec2 pos, glm::vec2 scale = glm::vec2(0.1, 0.1), glm::vec2 rot = glm::vec2(3.14, 0)) override;
	virtual float &GetColor() override { return m_color; }

	virtual bool MouseHoover(glm::vec2 mousePos) override;
	virtual bool &Clicked() { return clicked; };
	virtual int GetWinID()const override {};

	virtual glm::vec2 GetPosition() const override { return m_position; }
	virtual GuiText *GetText() { return m_text; }

private:
	glm::vec4 GetCoords();

private:
	GLuint m_vao;
	
	GuiText *m_text;

	glm::vec2 m_position = glm::vec2(0, 0);
	glm::vec2 m_scale;
	glm::vec2 m_rotation;
	float m_color = 0;


	bool clicked = false;

	unsigned int m_width;
	unsigned int m_height;
};

