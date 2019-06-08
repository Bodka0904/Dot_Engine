#pragma once
#include "Dot/Debug/Log.h"
#include "Dot/Core.h"


struct GLFWwindow;

class OpenGLContext
{
public:
	OpenGLContext(GLFWwindow* windowHandle);
	~OpenGLContext();

	void Init();
	void SwapBuffers();

private:
	GLFWwindow * m_WindowHandle;
};

