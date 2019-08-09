#pragma once
#include "Dot/Debug/Log.h"
#include "Dot/Core.h"

struct GLFWwindow;

namespace Dot {

	

	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext();

		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* m_WindowHandle;
	};

}

