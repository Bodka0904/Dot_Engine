#include "stdafx.h"
#include "Window.h"


static bool GLFWInitialized = false;

Window::Window(const WindowProps& props)
{
	m_data.width = props.Width;
	m_data.height = props.Height;
	m_data.title = props.Title;
	Init();

}

Window::~Window()
{
	glfwTerminate();
}

void Window::Init()
{
	if (!GLFWInitialized)
	{
		int success = glfwInit();
		D_ASSERT(success, "Could not initialize GLFW!");
		GLFWInitialized = true;
	}

	//glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title, NULL, NULL);
	GUI = WinGui::CreateWinGui(m_window,WinGuiParams(400,400,200,200));
	

	m_Context = new OpenGLContext(m_window);
	m_Context->Init();
	
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_data);

	
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		LOG_INFO("Window: Resized window width: %d height: %d", width, height);
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.width = width;
		data.height = height;
	
		glViewport(0, 0, width,height);
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

void Window::Update()
{
	
	glfwPollEvents();
	m_Context->SwapBuffers();
	
}

bool Window::IsClosed()
{
	
	return glfwWindowShouldClose(m_window);
	
}


void Window::Destroy()
{
	LOG_WARN("Window: Close")
	glfwDestroyWindow(m_window);
}



void Window::Clear()
{
	glClearColor(0.0f, 0.2f, 0.2f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Attach()
{
	glfwMakeContextCurrent(m_window);
}



inline int Window::GetWidth() const
{
	return m_data.width;
}

inline int Window::GetHeight() const
{
	return m_data.height;
}

GLFWwindow * Window::GetWindow() const
{
	return m_window;
}
