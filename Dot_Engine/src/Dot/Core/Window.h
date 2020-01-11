#pragma once
#include "stdafx.h"
#include "Events/Event.h"
#include <functional>


namespace Dot {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool FullScreen;
		bool Maximized;

		WindowProps(const std::string& title = "Dot Engine",
			unsigned int width = 1280,
			unsigned int height = 720,
			bool maximized = true,
			bool fullScreen = false)
			: Title(title), Width(width), Height(height),FullScreen(fullScreen),Maximized(maximized)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;
		virtual void Update() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsClosed() = 0;
		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
	

}