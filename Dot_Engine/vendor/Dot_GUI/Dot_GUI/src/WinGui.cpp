#include "stdafx.h"
#include "WinGui.h"




WinGui::WinGui(GLFWwindow* mainWnd, WinGuiParams params)
	: g_data(params)
{
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_window = glfwCreateWindow(g_data.width, g_data.height, "GUI", NULL, mainWnd);
}



void WinGui::Update()
{
	
}

void WinGui::Attach()
{
	glfwMakeContextCurrent(g_window);
}

GLFWwindow * WinGui::GetWindow() const
{
	return g_window;
}



	




