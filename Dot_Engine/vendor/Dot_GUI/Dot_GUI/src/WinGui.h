#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Dot/Log.h"
#include "Dot/Events/Event.h"



struct GuiWinProps
{
	const char* Title;
	unsigned int Width;
	unsigned int Height;

	GuiWinProps(const char* title = "Dot GUI",
		unsigned int width = 720,
		unsigned int height = 720)
		: Title(title), Width(width), Height(height)
	{
	}
};

class WinGui
{
public:
	WinGui(const GuiWinProps& props = GuiWinProps());
	~WinGui();
	void Init();
	
	void Clear();
	void Attach();
	void Update();
	
		
	static WinGui* Create(const GuiWinProps& props = GuiWinProps()) { return new WinGui(props); }

	void SetupGLFW();
	void SetupGL();

private:
	GLFWwindow * m_window;
	

	struct WindowData
	{
		const char* title;
		unsigned int width;
		unsigned int height;
		Event event;
	};

	WindowData m_data;

};




