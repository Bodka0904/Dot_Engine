#include "Window.h"


void ResizeWindow(GLFWwindow *window, int width, int height);

Window::Window(int width, int height, const char* title)
{

	m_width = width;
	m_height = height;
	m_title = title;
	Init();

	glewInit();

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);



}


Window::~Window()
{
	glfwTerminate();
}

void Window::Init()
{
	if (!glfwInit())
	{
		LOG_ERR("Window","Could not initialize GLFW")
		glfwTerminate();
	}

	LOG_INFO("Window","Creating window")
	LOG_INFO("Window","Width:" + m_width)
	LOG_INFO("Window","Height:" + m_height)
	m_window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);

	glfwMakeContextCurrent(m_window);
	glfwWindowHint(GLFW_DEPTH_BITS, 16);

	glfwSetFramebufferSizeCallback(m_window, ResizeWindow);


}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
	
}

bool Window::IsClosed()
{
	return glfwWindowShouldClose(m_window);
}


void Window::Terminate()
{
	glfwTerminate();
}

void Window::ShowWindow()
{
	glfwShowWindow(m_window);
}

void Window::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ResizeWindow(GLFWwindow *window,int width, int height)
{

	
	glViewport(0, 0, width, height);
	LOG_INFO("Window", "Resized width" + width)
	LOG_INFO("Window", "Resized height" + height)
	
}




inline int Window::GetWidth() const
{
	return m_width;
}

inline int Window::GetHeight() const
{
	return m_height;
}

GLFWwindow * Window::GetWindow() const
{
	return m_window;
}
