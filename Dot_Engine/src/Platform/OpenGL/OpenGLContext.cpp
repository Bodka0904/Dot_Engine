#include "stdafx.h"
#include "OpenGLContext.h"
#include <GLFW\glfw3.h>
#include <GL\glew.h>
#include <GL/GL.h>


OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	D_ASSERT(windowHandle, "Window handle is null!")
}


OpenGLContext::~OpenGLContext()
{
	
}

void OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);

	LOG_WARN("OpenGL Info:");
	LOG_WARN("Vendor: %s", (char*)glGetString(GL_VENDOR));
	LOG_WARN("Renderer: %s", (char*)glGetString(GL_RENDERER));
	LOG_WARN("Version: %s", (char*)glGetString(GL_VERSION));

	if (glewInit() != GLEW_OK)
	{
		LOG_ERR("OpenGLContext: Could not initialize glew")
	};
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
