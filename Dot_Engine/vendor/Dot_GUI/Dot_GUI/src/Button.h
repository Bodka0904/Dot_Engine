#pragma once
#include "Types.h"


class Button
{
public:
	Button(WidgetParams params);
	~Button();

	static Button* Create(WidgetParams params) { return new Button(params ); }
	void Draw();

private:
	WidgetParams button;
};

