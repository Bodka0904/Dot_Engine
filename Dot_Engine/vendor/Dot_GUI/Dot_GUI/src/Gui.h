#pragma once
#include "GuiShader.h"
#include "events/GuiEvent.h"
#include "glText/GuiText.h"
#include "GuiBuffer.h"
#include "data/GuiData.h"
#include "widgets/GuiWindow.h"
#include "widgets/GuiWidget.h"
#include "widgets/GuiButton.h"
#include "widgets/GuiCheckBox.h"
#include "widgets/GuiSlider.h"
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
	
	static void HandleWidgetClick(GuiEvent& event);
	static void HandleButtonCallbacks();

	static void HandleCheckBoxCallbacks();
	static void HandleReleaseButtons(GuiEvent& event);

	static void Gui_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void Gui_MousePositionCallback(GLFWwindow* window,double xPos, double yPos);
	static void Gui_WindowSizeCallback(GLFWwindow* window, int width, int height);
	
	
	static void AddWindow(const std::string& name);
	static void AddButton(func_ptr func,const std::string& name,int winID);
	static void AddCheckBox(func_ptr func, const std::string& name,int winID);
	static void AddSlider(const std::string& name, int winID);

	static void SetDarkTheme();

private:
	static std::vector<GuiWidget*> m_widgets;
	static std::vector<func_ptr> m_user_callbacks;


	static unsigned int num_windows;
	static unsigned int num_buttons;
	static unsigned int num_checkboxes;
	static unsigned int num_sliders;
	
	

	static GuiShader* guiShader;
	static GuiTransform *transform;
	static GLFWwindow* m_handler;

	static GLFWmousebuttonfun m_handler_mouseButtonCLB;
	static GLFWcursorposfun m_handler_cursorPosCLB;
	static GLFWframebuffersizefun m_handler_winSizeCLB;

	
	static bool EDIT_MODE;
	static int attachedWidget;

	static int winWidth;
	static int winHeight;

	static float m_mousePosX;
	static float m_mousePosY;
	
	static int m_counter;

	static GuiIndexBuffer* m_index;

	static GuiVertexBuffer* m_vertex_w;
	static GuiVertexBuffer* m_vertex_b;
	static GuiVertexBuffer* m_vertex_chb;
	static GuiVertexBuffer* m_vertex_sl;
	

};

