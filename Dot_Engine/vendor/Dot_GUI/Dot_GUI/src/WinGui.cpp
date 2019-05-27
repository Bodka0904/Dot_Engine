#include "stdafx.h"
#include "WinGui.h"




WinGui::WinGui(GLFWwindow* mainWnd, WinGuiParams params)
	: g_data(params)
{
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_window = glfwCreateWindow(g_data.width, g_data.height,g_data.title, NULL, mainWnd);
	glfwSetWindowUserPointer(g_window, this);

	glfwSetCursorEnterCallback(g_window, [](GLFWwindow * window, int entered)
	{
		WinGui* self = (WinGui*)glfwGetWindowUserPointer(window);
		self->OnCursorEnter(entered != GLFW_FALSE);
	});

}




void WinGui::Update()
{
	Button::Create(ButtonParams(10, 10, 10, 10,glm::vec3(0.5,0.5,0.5), "test"));
}



GLFWwindow * WinGui::GetWindow() const
{
	return g_window;
}



	




