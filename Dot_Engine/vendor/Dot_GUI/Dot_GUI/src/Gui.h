#pragma once
#include "GuiShader.h"
#include "events/GuiEvent.h"
#include "glText/GuiText.h"
#include "GuiButton.h"
#include <GLFW\glfw3.h>
#include "GuiTransform.h"



class Gui
{
public:

	typedef std::function<void()> func_ptr;
	

	static void Init(GLFWwindow* handler);


	static void Render();
	static void Update();
	static void Clear();
	


	static void HandleButtonCallbacks();
	static void HandlePressButtons(GuiEvent& event);
	static void HandleReleaseButtons(GuiEvent& event);
	


	static void Gui_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void Gui_MousePositionCallback(GLFWwindow* window,double xPos, double yPos);
	static void Gui_WindowSizeCallback(GLFWwindow* window, int width, int height);
	
	
	static void AddButton(func_ptr func,const std::string& name,glm::vec3 color);
	static GuiButton *GetButton(int index) { return m_buttons[index]; }

private:
	static std::vector<GuiButton*> m_buttons;
	static std::vector<func_ptr> m_user_callbacks_B;

	static GuiShader* guiShader;
	static GuiTransform *transform;
	static GLFWwindow* m_handler;
	static GLFWmousebuttonfun m_handler_mouseButtonCLB;
	static GLFWcursorposfun m_handler_cursorPosCLB;
	static GLFWframebuffersizefun m_handler_winSizeCLB;

	
	static bool EDIT_MODE;

	static int winWidth;
	static int winHeight;

	static float m_mousePosX;
	static float m_mousePosY;
	
	static int m_numWidgets;
};

