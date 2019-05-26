#pragma once
#include "Types.h"
#include "Button.h"


class Widget
{
public:
	void AddButton(WidgetParams& params) { buttons.push_back(Button::Create(params)); }



private:
	std::vector<Button*> buttons;
	
};