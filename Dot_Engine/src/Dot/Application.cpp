#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application()
	:
	texture(1)	
{
	
	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
	

	shader.Init("res/shaders/BasicShader");
	shader.Bind();
	mesh = Mesh::CreateMesh("res/models/player.obj");
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

		camera.GetRot().z -= 0.01;
		transform.GetPos().z -= 0.2;
		mesh->Draw();
		shader.Update(transform, camera);
		
		for (Layer* layer : m_Layers)
			layer->OnUpdate();
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
