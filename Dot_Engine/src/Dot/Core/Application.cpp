#include "stdafx.h"
#include "Application.h"
#include "Dot/Utils/Time/Timestep.h"
#include "Dot/Renderer/RenderCommand.h"
#include "Dot/Debug/Timer.h"

#include "Dot/Core/AssetManager.h"
#include "Dot/Utils/Parser/JsonParser.h"
#include "Dot/ECS/ECSManager.h"
#include <GLFW/glfw3.h>

#include "Dot/ParticleEngine/ParticleEffect.h"
#include "Dot/Renderer/Renderable.h"
#include "Dot/Gui/GuiSystem/Gui.h"

namespace Dot {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
	
		s_Instance = this;
		m_Window = Window::Create();

		m_Window->SetVSync(false);
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		
		m_GuiLayer = new GuiLayer();
		PushOverlay(m_GuiLayer);
	

		Dot::ECSManager::Get()->Init();
		Dot::ECSManager::Get()->RegisterComponent<AABB>();
		Dot::ECSManager::Get()->RegisterComponent<RigidBody>();
		Dot::ECSManager::Get()->RegisterComponent<Transform>();
		Dot::ECSManager::Get()->RegisterComponent<RenderComponent>();
		Dot::ECSManager::Get()->RegisterComponent<ParticleComponent>();
		Dot::ECSManager::Get()->RegisterComponent<Ref<ParticleEffect>>();

		AssetManager::Get()->LoadAssets("Assets.json");
	}


	Application::~Application()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
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
				{	
					//Timer timer;
					layer->OnUpdate(timestep);
					layer->OnGuiUpdate();
					
				}
			}
			
			
			m_Window->Update();	
		}
	}

	void Application::PushLayer(Layer * layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
		if (Gui::Get())
			layer->OnGuiAttach();
	}

	void Application::PushOverlay(Layer * overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
		if (Gui::Get())
			overlay->OnGuiAttach();
	}

	void Application::PopLayer(Layer * layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			if (Gui::Get())
				(*it)->OnGuiDetach();
			(*it)->OnDetach();
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
		if (event.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& resize = (WindowResizeEvent&)event;
			RenderCommand::SetViewport(0, 0, resize.GetWidth(), resize.GetHeight());
		}
		for (auto it = m_Layers.end()-1; it != m_Layers.begin(); )
		{
			if (Gui::Get())
			{
				(*it)->OnGuiEvent(event);
				if (event.IsHandled())
					break;
			}
			(*it)->OnEvent(event);
			--it;
			if (event.IsHandled())
				break;
		}
	}

}