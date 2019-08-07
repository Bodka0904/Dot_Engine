#pragma once
#include <functional>
#include <map>
#include "graphics/GuiShader.h"
#include "events/GuiEvent.h"
#include "glText/GuiText.h"
#include "graphics/GuiBuffer.h"
#include "data/GuiData.h"
#include "widgets/GuiWrapper.h"
#include "widgets/GuiWidget.h"
#include "widgets/GuiButton.h"
#include "widgets/GuiArrowButton.h"
#include "widgets/GuiCheckBox.h"
#include "widgets/GuiSlider.h"
#include "graphics/GuiTexture.h"
#include "graphics/GuiTransform.h"
#include <GLFW\glfw3.h>



class Gui
{
public:
	
	typedef std::function<void()> func_ptr;

	static void Init(GLFWwindow* handler);
	static bool Inited() { return Initialized; }

	static void Render();
	static void Update();
	static void Clear();



	static void HandleWrapperClick(GuiEvent& event);
	static void HandleWidgetClick(GuiEvent& event);
	 
	static void HandleReleaseWrapper(GuiEvent& event);
	static void HandleReleaseWidget(GuiEvent& event);
	

	static void Gui_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void Gui_MousePositionCallback(GLFWwindow* window,double xPos, double yPos);
	static void Gui_WindowSizeCallback(GLFWwindow* window, int width, int height);
	
	
	static void AddWrapper(const std::string& name);
	static GuiWrapper& GetWrapper(const std::string& name);
	static bool& IsHandlingEvent() { return HandlingEvent; }


private:
	static bool Initialized;
	static bool HandlingEvent;
	static std::map <std::string,std::unique_ptr<GuiWrapper>> m_wrappers;

	
	
	static std::unique_ptr<GuiShader> guiShader;
	static std::unique_ptr<GuiTransform> transform;
	 
	static std::unique_ptr<GuiTexture> wrp_texture;
	static std::unique_ptr<GuiTexture> btn_texture;
	static std::unique_ptr<GuiTexture> abtn_texture;
	static std::unique_ptr<GuiTexture> checkbox_texture;
	static std::unique_ptr<GuiTexture> slider_texture;

	static GLFWwindow* m_handler;
	

	static GLFWmousebuttonfun m_handler_mouseButtonCLB;
	static GLFWcursorposfun m_handler_cursorPosCLB;
	static GLFWframebuffersizefun m_handler_winSizeCLB;

	static bool EDIT_WRAPPER;
	static std::string attachedWrapper;

	static bool EDIT_WIDGET;
	static std::pair<std::string, int> attachedWidget;

	static int winWidth;
	static int winHeight;

	static float m_mousePosX;
	static float m_mousePosY;
	static int m_left_btn_counter;
	static int m_right_btn_counter;

	static std::shared_ptr<GuiIndexBuffer> m_index;
	static std::shared_ptr<GuiVertexBuffer> m_vertex_w;
	static std::shared_ptr<GuiVertexBuffer> m_vertex_b;
	static std::shared_ptr<GuiVertexBuffer> m_vertex_ab;
	static std::shared_ptr<GuiVertexBuffer> m_vertex_chb;
	static std::shared_ptr<GuiVertexBuffer> m_vertex_sl;


};

