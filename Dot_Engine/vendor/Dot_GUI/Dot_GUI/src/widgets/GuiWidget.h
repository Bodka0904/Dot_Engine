#pragma once
#include "../GuiShader.h"
#include "../GuiTransform.h"
#include "../glText/GuiText.h"

class GuiWidget
{
public:
	GuiWidget() {};
	virtual ~GuiWidget() {};

	virtual void Init(unsigned int& VBO,unsigned int& IBO) = 0;
	virtual void Draw(GuiShader& shader,GuiTransform& transform) = 0;
	virtual void UpdateData(GuiTransform& transform) = 0;
	
	virtual void SetData(glm::vec2 pos, glm::vec2 scale = glm::vec2(1, 1)) = 0;
	virtual float &GetColor() = 0;
	
	virtual bool MouseHoover(glm::vec2 mousePos) = 0;
	virtual bool &Clicked() = 0;

	virtual glm::vec2 GetPosition() const = 0;
	virtual GuiText *GetText() = 0;


};
