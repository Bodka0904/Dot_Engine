#pragma once
#include "stdafx.h"
#include "shaders/GuiShader.h"
#include "GuiButton.h"
#include <GLFW\glfw3.h>


class Gui
{
public:
	static void Init(GLFWwindow* handler, int widthWin, int heightWin);

	static void Render();
	static void Update();
	static void Clear();

	static void UpdateWidgets();
	static void Gui_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void Gui_MousePositionCallback(GLFWwindow* window,double xPos, double yPos);
	static void Gui_WindowSizeCallback(GLFWwindow* window, int width, int height);
	
	static void AddButton() { m_buttons.push_back(new GuiButton()); }
	static GuiButton *GetButton(int index) { return m_buttons[index]; }

private:
	static std::vector<GuiButton*> m_buttons;

	static GuiShader* guiShader;
	static GLFWwindow* m_handler;
	static GLFWmousebuttonfun m_handler_mouseButtonCLB;
	static GLFWcursorposfun m_handler_cursorPosCLB;
	static GLFWframebuffersizefun m_handler_winSizeCLB;

	
	static int winWidth;
	static int winHeight;
	

private:
	static bool m_mouseButtonPressedL;
	static bool m_mouseButtonPressedR;
	static int m_mousePressesR;
	static int *test;

	static float m_mousePosX;
	static float m_mousePosY;
	
};

