#pragma once
#include "Graphics/Window.h"
#include "Layer.h"
#include "Events/Event.h"
#include "Gui/GuiLayer.h"
#include "Dot/Graphics/Text/Text.h"
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Camera/OrthoCamera.h"

namespace Dot {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
		void OnEvent(Event &e);


		Window GetWin() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
		static Application *Create() { return new Application(); }

	private:
		std::unique_ptr<Window> m_Window;
		std::vector<Layer*> m_Layers;
		GuiLayer *m_GuiLayer;

		std::shared_ptr<Shader> shader;
		std::shared_ptr<OrthoCamera> camera;

		std::shared_ptr<Text> text;

		unsigned int m_LayerInsertIndex = 0;
		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;

		static Application* s_Instance;

	private:
		float m_LastFrameTime = 0;
	
	};

	// To be defined in CLIENT
	Application *CreateApplication();

}