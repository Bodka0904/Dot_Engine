#pragma once


namespace Dot {
	class APIContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<APIContext> Create(void* window);
	};
}