#pragma once
#include "GuiWidget.h"
#include "Dot_GUI/data/GuiData.h"


class GuiSlider : public GuiWidget
{
public:

	GuiSlider(const std::string& name);
	virtual ~GuiSlider() override;

	virtual void Init(unsigned int& VBO, unsigned int& IBO) override;
	virtual void Draw(GuiShader& shader, GuiTransform& transform) override;

	virtual void SetData(glm::vec2 pos, glm::vec2 scale = glm::vec2(1, 1)) override;
	virtual void UpdateData(GuiTransform& transform) override;
	virtual void SetValue(float value) override;
	

	virtual bool &Clicked() override { return clicked; }
	virtual float GetValue() const override {return m_value*100; }


	virtual bool MouseHoover(glm::vec2 mousePos) override;

	virtual glm::vec2 GetPosition() const override { return m_position; };
	virtual GuiText *GetText() override { return m_text; }

private:
	glm::vec4 GetCoords();
	virtual bool &LeftClicked() override { return clicked; };
	virtual bool &RightClicked() override { return clicked; };

private:
	GLuint m_vao;


private:
	GuiText *m_text;

	glm::vec2 m_position = glm::vec2(0, 0);
	glm::vec2 m_scale;

	float m_value = 1.0f;

	bool clicked = false;
};

