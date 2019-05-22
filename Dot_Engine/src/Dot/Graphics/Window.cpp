#include "Window.h"


static bool GLFWInitialized = false;
void ResizeWindow(GLFWwindow *window, int width, int height);

Window::Window(const WindowProps& props)
{

	m_data.width = props.Width;
	m_data.height = props.Height;
	m_data.title = props.Title;
	Init();
	glewExperimental = true;
	glewInit();
	glViewport(0, 0, m_data.width, m_data.height);
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
	if (!GLFWInitialized)
	{
		int success = glfwInit();
		D_ASSERT(success, "Could not initialize GLFW!");
		GLFWInitialized = true;
	}

	m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title, NULL, NULL);
	LOG_INFO("Window: Created window width:%d height:%d", m_data.width, m_data.height);
	
	
	glfwMakeContextCurrent(m_window);
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwSetFramebufferSizeCallback(m_window, ResizeWindow);
	
	LOG_WARN((char*)glGetString(GL_VENDOR));
	LOG_WARN((char*)glGetString(GL_RENDERER));
	LOG_WARN((char*)glGetString(GL_VERSION));

}

void Window::Update()
{

	glfwPollEvents();
	glfwSwapBuffers(m_window);

}

bool Window::IsClosed()
{
	LOG_INFO("Window: Close")
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
