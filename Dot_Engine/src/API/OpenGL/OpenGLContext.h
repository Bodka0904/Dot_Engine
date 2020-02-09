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

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;

		bool anotherWin = false;
	};

}

