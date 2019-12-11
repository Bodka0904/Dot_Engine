#include "stdafx.h"
#include "Application.h"
#include "Dot/Utils/Time/Timestep.h"
#include "Dot/Utils/Text/Font.h"

#include "Dot/Debug/Timer.h"
#include <GLFW/glfw3.h>

namespace Dot {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
	
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());

		m_Window->vSync(false);
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		
		//Default font
		Font::AddFont("Arial", "res/fonts/Arial/Arial.fnt", "res/fonts/Arial/Arial.png");
	}


	Application::~Application()
	{

		for (Layer* layer : m_Layers)
			delete layer;


	}

	void Application::Run()
	{

		// TODO PUT BACK TO APPLICATION CONSTRUCTOR
		m_GuiLayer = new GuiLayer();
		m_ConsoleLayer = new ConsoleLayer();
		PushOverlay(m_GuiLayer);
		PushOverlay(m_ConsoleLayer);

		while (!m_Window->IsClosed())
		{
			
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_Layers)
			{
				{	
					//Timer timer;
					layer->OnUpdate(timestep);
					
				}
			}
			
			
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
			if (event.IsHandled())
				break;
		}
	}

}