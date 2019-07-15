#pragma once
#include "GuiWidget.h"
#include "Dot_GUI/data/GuiData.h"
#include "Dot_GUI/GuiShader.h"
#include "Dot_GUI/GuiBuffer.h"
#include "Dot_GUI/GuiTransform.h"
#include "Dot_GUI/glText/GuiText.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>


class GuiArrowButton : public GuiWidget
{
public:
	GuiArrowButton(const std::string& name);
	virtual ~GuiArrowButton();

	virtual void Init(unsigned int& VBO, unsigned int& IBO)override;
	virtual void Draw(GuiShader& shader, GuiTransform& transform)override;


	virtual void UpdateData(GuiTransform& transform)override;
	virtual void SetData(glm::vec2& pos, glm::vec2& scale = glm::vec2(1, 1))override;
	

	virtual void SetValue(float value) override { m_color = value; }	
	virtual float GetValue() const override { return m_color; }


	virtual bool MouseHoover(glm::vec2 mousePos)override;
	virtual bool &LeftClicked() override { return m_left_click; };
	virtual bool &RightClicked() override { return m_right_click; };

	virtual glm::vec2 GetPosition() const override { return m_position; }
	virtual GuiText *GetText() override { return m_text; }

private:
	glm::vec4 GetCoords();
	virtual bool &Clicked()override { return *m_click;};


private:
	GLuint m_vao;


private:
	GuiText *m_text;

	glm::vec2 m_position = glm::vec2(0, 0);
	glm::vec2 m_scale;

	float m_color = 0.0f;

	bool *m_click;

	bool m_left_click = false;
	bool m_right_click = false;
};

