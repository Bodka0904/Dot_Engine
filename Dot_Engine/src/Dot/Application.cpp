#include "stdafx.h"
#include "Application.h"
#include "Dot_GUI/src/Gui.h"
#include "Dot/Utils/Timestep.h"
#include "Dot/Graphics/Text/Text2D.h"
#include "Dot/Graphics/Text/Font.h"

namespace Dot {

	//0th argument stands for Function to call,
	//1th must be called because it is class member function,
	//2th is placeholder for agument
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{

		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());

		m_Ter_editor_layer = new TerrainEditorLayer();
		PushOverlay(m_Ter_editor_layer);

		m_GuiLayer = new GuiLayer();
		PushOverlay(m_GuiLayer);

		
		m_Window->vSync(false);
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		
		InitText2D("res/fonts/Arial.DDS");

		Font::AddFont("Arial","res/fonts/Arial.DDS");
		text.reset(new Text("TEST", 20, 50, 12));

		if (!Gui::Inited())
		{
			Gui::Init(m_Window->GetWindow());
		}
	}


	Application::~Application()
	{

		for (Layer* layer : m_Layers)
			delete layer;


	}

	void Application::Run()
	{
		
		
		while (!m_Window->IsClosed())
		{
			
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			for (Layer* layer : m_Layers)
			{
				layer->OnUpdate(timestep);
			}
			printText2D("Hello", 20, 50, 12);
			text->PrintText("Arial");
			m_Window->Update();

			
			
		}
	
		m_Window->Destroy();

	}

	void Application::PushLayer(Layer * layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer * overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer * layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void Application::PopOverlay(Layer * overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

	void Application::OnEvent(Event & event)
	{
		for (auto it = m_Layers.end(); it != m_Layers.begin(); )
		{
			(*--it)->OnEvent(event);
		}
	}

}