#include "stdafx.h"
#include "Input.h"
#include "Application.h"


Input* Input::s_Instance = new Input();

std::pair<float, float> Input::GetMousePositionImp1()
{
	GLFWwindow* window = Application::Get().GetWin().GetWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

bool Input::IsKeyPressedImp1(int keycode)
{
	GLFWwindow* window = Application::Get().GetWin().GetWindow();
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressedImp1(int button)
{
	GLFWwindow* window = Application::Get().GetWin().GetWindow();
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

float Input::GetMouseXImp1()
{
	auto[x, y] = Input::GetMousePositionImp1();
	return x;
}

float Input::GetMouseYImp1()
{

	auto[x, y] = Input::GetMousePositionImp1();
	return y; 0.0f;
}