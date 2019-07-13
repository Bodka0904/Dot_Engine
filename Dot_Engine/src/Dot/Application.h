#pragma once
#include "Graphics/Window.h"
#include "Layer.h"
#include "Events/Event.h"
#include "Gui/GuiLayer.h"
#include "Dot/Terrain/TerrainEditorLayer.h"
#include "Debug/Timer.h"



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
		TerrainEditorLayer* m_Ter_editor_layer;
		

		unsigned int m_LayerInsertIndex = 0;
		unsigned int m_VertexArray;
		unsigned int m_VertexBuffer;
		unsigned int m_IndexBuffer;

		static Application* s_Instance;

	};

	// To be defined in CLIENT
	Application *CreateApplication();

}