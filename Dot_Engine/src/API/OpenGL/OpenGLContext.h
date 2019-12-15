#pragma once
#include "Dot/Debug/Log.h"
#include "Dot/Renderer/APIContext.h"

struct GLFWwindow;

namespace Dot {

	class OpenGLContext : public APIContext
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

