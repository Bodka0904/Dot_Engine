#include "stdafx.h"
#include "Application.h"
#include "Dot/Utils/Time/Timestep.h"
#include "Dot/Renderer/RenderCommand.h"
#include "Dot/Debug/Timer.h"

#include "Dot/Core/AssetManager.h"
#include "Dot/Utils/Parser/JsonParser.h"
#include "Dot/ECS/ECSManager.h"


#include "Dot/Renderer/Renderable/Renderable.h"
#include "Dot/LevelEditor/Terrain/Terrain.h"
#include "Dot/ParticleEngine/ParticleComponent.h"

#include <GLFW/glfw3.h>

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
		Dot::ECSManager::Get()->RegisterComponent<ParticleEmitter>();
		Dot::ECSManager::Get()->RegisterComponent<SizeOverLifeTime>();
		Dot::ECSManager::Get()->RegisterComponent<VelocityOverLifeTime>();
		Dot::ECSManager::Get()->RegisterComponent<ComputeComponent>();

		AssetManager::Get()->LoadAssets("Assets.json");
	}


	Application::~Application()
	{
		for (Layer* layer : m_Layouters)
		{
			layer->OnGuiDetach();
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

			for (Layer* layer : m_Layouters)
			{
				{	
					//Timer timer;
					layer->OnGuiUpdate();
					layer->OnUpdate(timestep);				
				}
			}
			m_Window->Update();	
		}
	}

	void Application::PushLayer(Layer * layer)
	{
		m_Layouters.emplace(m_Layouters.begin() + m_LayouterInsertIndex, layer);
		m_LayouterInsertIndex++;
		layer->OnAttach();
		layer->OnGuiAttach();
	}

	void Application::PushOverlay(Layer * overlay)
	{
		m_Layouters.emplace_back(overlay);
		overlay->OnAttach();
		
		overlay->OnGuiAttach();
	}

	void Application::PopLayer(Layer * layer)
	{
		auto it = std::find(m_Layouters.begin(), m_Layouters.begin() + m_LayouterInsertIndex, layer);
		if (it != m_Layouters.begin() + m_LayouterInsertIndex)
		{
			
			(*it)->OnGuiDetach();
			(*it)->OnDetach();
			
			m_Layouters.erase(it);
			m_LayouterInsertIndex--;
		}
	}

	void Application::PopOverlay(Layer * overlay)
	{
		auto it = std::find(m_Layouters.begin() + m_LayouterInsertIndex, m_Layouters.end(), overlay);
		if (it != m_Layouters.end())
			m_Layouters.erase(it);
	}

	void Application::OnEvent(Event & event)
	{
		if (event.GetEventType() == EventType::WindowResized)
		{
			WindowResizeEvent& resize = (WindowResizeEvent&)event;
			RenderCommand::SetViewport(0, 0, resize.GetWidth(), resize.GetHeight());
		}
		for (auto it = m_Layouters.rbegin(); it != m_Layouters.rend(); ++it )
		{					
			(*it)->OnEvent(event);
			if (event.IsHandled())
				break;
		}
	}

}