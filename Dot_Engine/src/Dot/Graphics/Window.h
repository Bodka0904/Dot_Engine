#pragma once
#include <GL\glew.h>
#include "Platform/OpenGL/OpenGLContext.h"
#include <GLFW\glfw3.h>
#include "Dot/Events/Event.h"
#include "Dot/Debug/Log.h"
#include "Dot/Core.h"





struct WindowProps
{
	const char* Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const char* title = "Dot Engine",
		unsigned int width = 1280,
		unsigned int height = 720)
		: Title(title), Width(width), Height(height)
	{
	}
};

class Window
{
public:
	//It is function that takes Event and return nothing
	using EventCallbackFn = std::function<void(Event&)>;
	

	Window(const WindowProps& props = WindowProps());
	virtual~Window();
	void Init();
	void Update();
	bool IsClosed();
	void Destroy();
	void Clear();
	void Attach();



	//I can set what this function will represent
	void SetEventCallback(const EventCallbackFn& callback) { m_data.EventCallback = callback; }

	inline int GetWidth() const;
	inline int GetHeight() const;

	static Window* Create(const WindowProps& props = WindowProps()) { return new Window(props); }

	GLFWwindow* GetWindow() const;
	
	void vSync(bool enabled);

private:
	GLFWwindow * m_window;

	struct WindowData
	{
		const char* title;
		unsigned int width;
		unsigned int height;

		EventCallbackFn EventCallback;
	};

	WindowData m_data;
	OpenGLContext* m_Context;


};