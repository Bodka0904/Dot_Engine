#pragma once
#include "GuiWidget.h"
#include "graphics/GuiArrayBuffer.h"



class GuiSlider : public GuiWidget
{
public:

	GuiSlider(const std::string& name,const glm::vec2& size = glm::vec2(150,20));
	virtual ~GuiSlider() override;

	virtual void Draw(GuiShader& shader, GuiTransform& transform) override;

	virtual void SetData(glm::vec2& pos) override;

	virtual void UpdateData(GuiTransform& transform) override;
	virtual void SetValue(float value) override;
	

	virtual bool &Clicked() override { return clicked; }
	virtual float GetValue() const override {return (1-m_value)*100; }


	virtual bool MouseHoover(glm::vec2 mousePos) override;

	virtual glm::vec2 GetPosition() const override { return m_position; };
	virtual glm::vec2 GetSize() const override { return m_size; }
	virtual GuiText *GetText() override { return m_text; }

private:
	glm::vec4 GetCoords();
	virtual bool &LeftClicked() override { return clicked; };
	virtual bool &RightClicked() override { return clicked; };

private:
	std::shared_ptr<GuiArrayBuffer> m_VAO;


private:
	GuiText *m_text;

	glm::vec2 m_position = glm::vec2(0, 0);
	glm::vec2 m_size;

	float m_value = 1.0f;

	bool clicked = false;
};

