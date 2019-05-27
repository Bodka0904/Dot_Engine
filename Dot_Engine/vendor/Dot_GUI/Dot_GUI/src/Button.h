#pragma once
#include "Types.h"


class Button
{
public:
	Button();
	~Button();

	static ButtonParams* Create(ButtonParams params) { return new ButtonParams(params ); }
	static void Init();
	static void Draw();
	static void SetColor();
	static void SetSize();

	static void AddButton(ButtonParams* button);

private:
	static std::vector<ButtonParams*> buttons;
	static int NumButtons;
	

	enum {
		POSITION_VB,

		COLOR_VB,

		OFFSET_VB,

		NUM_BUFFERS
	};
	static GLuint VAO;
	static GLuint VBO[NUM_BUFFERS];
};

