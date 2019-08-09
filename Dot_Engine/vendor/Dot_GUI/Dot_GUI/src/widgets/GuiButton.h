#pragma once
#include "GuiWidget.h"


#include "graphics/GuiTransform.h"
#include "glText/GuiText.h"
#include "graphics/GuiArrayBuffer.h"
#include <glm/glm.hpp>
#include <string>





class GuiButton : public GuiWidget
{
public:

	GuiButton(const std::string& name,const glm::vec2& size = glm::vec2(60,40));
	virtual ~GuiButton();
	
	virtual void Draw(GuiShader& shader, GuiTransform& transform) override;


	virtual void UpdateData(GuiTransform& transform) override;
	virtual void SetData(const glm::vec2& pos) override;
	
	virtual void SetValue(float value) override { m_color = value; }

	virtual float GetValue() const override { return m_color; }
	virtual bool MouseHoover(const glm::vec2& mousePos) override;
	virtual bool &Clicked() override { return clicked; }
	


	virtual glm::vec2 GetPosition() const override { return m_position; }
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

	glm::vec2 m_position = glm::vec2(0,0);
	glm::vec2 m_size;

	float m_color = 0.0f;
	bool clicked = false;

};

