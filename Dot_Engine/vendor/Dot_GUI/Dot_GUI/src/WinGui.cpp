#include "stdafx.h"
#include "WinGui.h"



WinGui::WinGui(const GuiWinProps& props)
{
	m_data.width = props.Width;
	m_data.height = props.Height;
	m_data.title = props.Title;
	Init();
}


WinGui::~WinGui()
{
}

void WinGui::Init()
{
	

	SetupGLFW();
	SetupGL();
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_data.height = mode->height;
	m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title, NULL,NULL);

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_data);
	
	Clear();
	Update();

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		LOG_INFO("GUI: Resized WinGui width: %d height: %d", width, height);
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;
		glViewport(0, 0, width, height);
	});
	
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{

		case GLFW_PRESS:
		{
			data.event.Handled = false;
			data.event.GetEventType() = EventType::KeyPressed;
			data.event.GetKeyCode() = key;
			data.event.Handled = true;
			if (data.event.Handled)
			{
				break;
			}
		}
		case GLFW_RELEASE:
		{
			data.event.Handled = false;
			data.event.GetEventType() = EventType::KeyReleased;
			data.event.GetKeyCode() = key;
			if (data.event.Handled)
			{
				break;
			}
		}

		}
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			data.event.Handled = false;
			data.event.GetEventType() = EventType::MouseButtonPressed;
			data.event.GetMouseButton() = button;
			if (data.event.Handled)
			{
				break;
			}
		}
		case GLFW_RELEASE:
		{
			data.event.Handled = false;
			data.event.GetEventType() = EventType::MouseButtonReleased;
			data.event.GetMouseButton() = button;
			if (data.event.Handled)
			{
				break;
			}
		}
		}
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		data.event.GetEventType() = EventType::MouseMoved;
		data.event.GetMouseX() = xPos;
		data.event.GetMouseY() = yPos;

	});
	
}

void WinGui::SetupGLFW()
{
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
}

void WinGui::SetupGL()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void WinGui::Clear()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WinGui::Attach()
{
	glfwMakeContextCurrent(m_window);
}

void WinGui::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}


