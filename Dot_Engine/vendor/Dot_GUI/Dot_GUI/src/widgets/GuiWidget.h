#pragma once
#include "graphics/GuiShader.h"
#include "graphics/GuiTransform.h"
#include "glText/GuiText.h"


class GuiWidget
{
public:
	

	GuiWidget() {};
	virtual ~GuiWidget() {};

	virtual void Draw(GuiShader& shader,GuiTransform& transform) = 0;
	virtual void UpdateData(GuiTransform& transform) = 0;
	
	virtual void SetData(glm::vec2& pos) = 0;
	
	virtual void SetValue(float value) = 0;

	virtual float GetValue() const = 0;
	virtual bool &Clicked() = 0;
	
	virtual bool MouseHoover(glm::vec2 mousePos) = 0;

	virtual bool &LeftClicked() = 0;
	virtual bool &RightClicked() = 0;

	virtual glm::vec2 GetPosition() const = 0;
	virtual glm::vec2 GetSize() const = 0;
	virtual GuiText *GetText() = 0;


};
