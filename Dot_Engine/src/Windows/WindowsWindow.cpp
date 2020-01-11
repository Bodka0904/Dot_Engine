#include "stdafx.h"
#include "WindowsWindow.h"

#include "Dot/Core/Events/Event.h"

namespace Dot {
	static bool GLFWInitialized = false;

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return std::make_unique<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}
	void WindowsWindow::Update()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);
	}


	bool WindowsWindow::IsClosed()
	{
		return glfwWindowShouldClose(m_Window);
	}


	void WindowsWindow::Init(const WindowProps& props)
	{
		if (!GLFWInitialized)
		{
			int success = glfwInit();

			D_ASSERT(success, "Could not initialize GLFW!");
			GLFWInitialized = true;
		}


		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int width = mode->width;
		int height = mode->height;

		if (props.Maximized)
		{
			glfwWindowHint(GLFW_MAXIMIZED, true);
			m_Window = glfwCreateWindow(width, height, props.Title.c_str(), NULL, NULL);
		}
		else if (props.FullScreen)
			m_Window = glfwCreateWindow(width, height, props.Title.c_str(), glfwGetPrimaryMonitor(), NULL);
		else
			m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);


		m_Context = APIContext::Create(m_Window);
		m_Context->Init();
		

		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent e(width, height);
			data.EventCallback(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent ev(key,mods);
				data.EventCallback(ev);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent ev(key);
				data.EventCallback(ev);
				break;
			}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressEvent e(button);
				data.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent e(button);
				data.EventCallback(e);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrollEvent e(yOffset);
			data.EventCallback(e);

		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			//MouseMovedEvent e((float)xPos, (float)yPos);
			//data.EventCallback(e);
		});
	}
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}