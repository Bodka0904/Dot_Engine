#pragma once
#include <functional>
#include "GuiShader.h"
#include "events/GuiEvent.h"
#include "glText/GuiText.h"
#include "GuiBuffer.h"
#include "data/GuiData.h"
#include "widgets/GuiWrapper.h"
#include "widgets/GuiWidget.h"
#include "widgets/GuiButton.h"
#include "widgets/GuiArrowButton.h"
#include "widgets/GuiCheckBox.h"
#include "widgets/GuiSlider.h"
#include "GuiTexture.h"
#include "GuiTransform.h"
#include <GLFW\glfw3.h>



class Gui
{
public:

	typedef std::function<void()> func_ptr;

	static void Init(GLFWwindow* handler);
	
	static void Render();
	static void Update();
	static void Clear();

	
	static void HandleWrapperClick(GuiEvent& event);
	static void HandleWidgetClick(GuiEvent& event);

	static void HandleReleaseWrapper(GuiEvent& event);
	static void HandleReleaseWidget(GuiEvent& event);
	static void HandleReleaseButtons(GuiEvent& event);

	static void HandleButtonCallbacks();
	static void HandleCheckBoxCallbacks();
	
	static void HandleSliders();

	static void Gui_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void Gui_MousePositionCallback(GLFWwindow* window,double xPos, double yPos);
	static void Gui_WindowSizeCallback(GLFWwindow* window, int width, int height);
	
	
	static void AddWrapper();
	static void AddButton(func_ptr func,const std::string& name);
	static void AddArrowButton(func_ptr func, const std::string& name);
	static void AddCheckBox(func_ptr func, const std::string& name);
	static void AddSlider(const std::string& name);

	static GuiWidget& GetButton(int index);
	static GuiWidget& GetArrowButton(int index);
	static GuiWidget& GetCheckBox(int index);
	static GuiWidget& GetSlider(int index);

	static void SetDarkTheme();

private:
	static std::vector<GuiWrapper*> m_wrappers;
	static unsigned int lastWidgetID;

	static std::vector<GuiWidget*> m_widgets;
	static std::vector<func_ptr> m_user_callbacks;


	static unsigned int num_buttons;
	static unsigned int num_checkboxes;
	static unsigned int num_sliders;
	static unsigned int num_arrow_btns;
	
	
	static GuiShader* guiShader;
	static GuiTransform *transform;

	static GuiTexture* wrp_texture;
	static GuiTexture* btn_texture;
	static GuiTexture* abtn_texture;
	static GuiTexture* checkbox_texture;

	static GLFWwindow* m_handler;


	static GLFWmousebuttonfun m_handler_mouseButtonCLB;
	static GLFWcursorposfun m_handler_cursorPosCLB;
	static GLFWframebuffersizefun m_handler_winSizeCLB;

	static bool EDIT_WRAPPER;
	static int attachedWrapper;

	static bool EDIT_WIDGET;
	static int attachedWidget;

	static int winWidth;
	static int winHeight;

	static float m_mousePosX;
	static float m_mousePosY;
	static int m_left_btn_counter;
	static int m_right_btn_counter;


	static GuiIndexBuffer* m_index;

	static GuiVertexBuffer* m_vertex_w;
	static GuiVertexBuffer* m_vertex_b;
	static GuiVertexBuffer* m_vertex_ab;
	static GuiVertexBuffer* m_vertex_chb;
	static GuiVertexBuffer* m_vertex_sl;
	
	
	
	

	
	

};

