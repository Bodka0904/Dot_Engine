#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Types.h"
#include "Button.h"
#include "Dot/Log.h"



class WinGui
{
public:
	WinGui(GLFWwindow* mainWnd,WinGuiParams params);
	void Update();
	
	void OnCursorEnter(bool entered) 
	{
		Update();
	}

	
	static WinGui* CreateWinGui(GLFWwindow* mainWnd,WinGuiParams params) { return new WinGui(mainWnd,params); }
	
	GLFWwindow* GetWindow() const;
	

private:
	GLFWwindow * g_window;
	WinGuiParams g_data;

};




