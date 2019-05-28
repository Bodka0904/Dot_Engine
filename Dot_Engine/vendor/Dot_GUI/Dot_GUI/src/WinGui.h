#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Types.h"
#include "Button.h"
#include "Dot/Log.h"



class WinGui
{
public:
	WinGui(WinGuiParams params);
	
	void InitWidgets();
	void DrawWidgets();
	void Update();
	void AddButton(ButtonParams params) { buttons.push_back(new Button(params));}

	void OnCursorEnter(bool entered) 
	{
		Update();
	}

	
	static WinGui* CreateWinGui(WinGuiParams params) { return new WinGui(params); }
	
	GLFWwindow* GetWindow() const;
	

private:
	GLFWwindow * g_window;
	WinGuiParams g_data;

	std::vector<Button*> buttons;
	GLuint button_VBO[3];


};




