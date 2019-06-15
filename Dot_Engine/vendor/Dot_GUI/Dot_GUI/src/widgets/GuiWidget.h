#pragma once
#include "../GuiTransform.h"
#include "../glText/GuiText.h"

class GuiWidget
{
public:
	GuiWidget() {};
	virtual ~GuiWidget() {};

	virtual void Init(unsigned int& VBO,unsigned int& IBO) = 0;
	virtual void Draw() = 0;
	virtual void UpdateData(GuiTransform& transform) = 0;
	virtual void SetData(glm::vec2 pos, glm::vec2 scale = glm::vec2(0.1, 0.1), glm::vec2 rot = glm::vec2(3.14, 0)) = 0;
	

	virtual bool MouseHoover(glm::vec2 mousePos) = 0;
	virtual bool &Clicked() = 0;

	virtual glm::vec2 GetPosition() const = 0;
	virtual GuiText *GetText() = 0;


};

