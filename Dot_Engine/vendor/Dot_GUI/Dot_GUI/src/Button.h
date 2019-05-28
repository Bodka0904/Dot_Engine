#pragma once
#include "Types.h"


class Button
{
public:
	Button(ButtonParams Params);
	~Button();

	void Draw();
	void SetColor();
	void SetSize();

	ButtonParams& GetParams() { return params; }


private:
	ButtonParams params;
	
};

