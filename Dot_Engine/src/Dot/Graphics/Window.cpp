#include "Window.h"


static bool GLFWInitialized = false;
void ResizeWindow(GLFWwindow *window, int width, int height);

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
	m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title, NULL, NULL);
	
	m_Context = new OpenGLContext(m_window);
	m_Context->Init();
	
	glfwSetFramebufferSizeCallback(m_window, ResizeWindow);

	glfwSetWindowUserPointer(m_window, &m_data);

	

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

void ResizeWindow(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	LOG_INFO("Window: Resized window width: %d height: %d", width, height);
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
