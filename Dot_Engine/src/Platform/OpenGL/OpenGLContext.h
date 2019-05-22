#pragma once
#include "Dot/Log.h"
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

