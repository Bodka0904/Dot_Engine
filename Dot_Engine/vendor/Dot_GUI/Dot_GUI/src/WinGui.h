#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Types.h"
#include "Widget.h"
#include "Button.h"



class WinGui
{
public:
	WinGui(GLFWwindow* mainWnd,WinGuiParams params);
	
	void Update();
	
	void Attach();

	Widget &GetWidgets() { return g_widgets; }
	static WinGui* CreateWinGui(GLFWwindow* mainWnd,WinGuiParams params) { return new WinGui(mainWnd,params); }
	
	GLFWwindow* GetWindow() const;


private:
	GLFWwindow * g_window;
	WinGuiParams g_data;
	Widget g_widgets;

};




