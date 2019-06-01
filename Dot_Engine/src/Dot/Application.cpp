#include "stdafx.h"
#include "Application.h"



Application* Application::s_Instance = nullptr;

Application::Application()
	:
	texture(1)
{

	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
	

	Gui::AddButton();
	Gui::Init(m_Window->GetWindow(),m_Window->GetWidth(),m_Window->GetHeight());
	
	


	shader.Init("res/shaders/BasicShader");
	mesh = Mesh::CreateMesh("res/models/test.obj");
	texture.Create("res/textures/robotTex.jpg");
	texture.Bind(0);


	

	
	
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
		
		m_Window->Clear();
		
		shader.Bind();
		transform.GetPos().z = -100;
		transform.GetRot().y += 0.01;
		mesh->Draw();
		shader.Update(transform, camera);
		
		
		Gui::Render();
		Gui::Update();
		


		for (Layer* layer : m_Layers)
		{
			layer->OnUpdate();
		}
		

		
		OnEvent(m_Window->GetEvent());
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
		if (event.Handled)
			break;
	}
}

