#pragma once
#include "OpenGLContext.h"
#include "Events/Event.h"
#include "Core.h"
#include <functional>

namespace Dot {

	struct WindowProps
	{
		const char* Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const char* title = "Dot Engine",
			unsigned int width = 1240,
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


		//I can set what this function will represent
		void SetEventCallback(const EventCallbackFn& callback) { m_data.EventCallback = callback; }

		int GetWidth() const;
		int GetHeight() const;

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

}