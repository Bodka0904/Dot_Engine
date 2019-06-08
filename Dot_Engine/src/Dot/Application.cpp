#include "stdafx.h"
#include "Application.h"

//0th argument stands for Function to call,
//1th must be called because it is class member function,
//2th is placeholder for agument
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application()
	:
	texture(1)
{
	
	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
	
	
	m_GuiLayer = new GuiLayer();
	PushOverlay(m_GuiLayer);
	


	//It makes from OnEvent() callback that is invoked when glfwwindow receive some callback
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	
	
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
	
	Timer time;
	float timer = 0;
	unsigned int frames = 0;
	while (!m_Window->IsClosed())
	{
	
		m_Window->Clear();
		
		shader.Bind();
		texture.Bind(0);
		transform.GetPos().z = -100;
		transform.GetRot().y += 0.01;
		
		mesh->Draw();
		shader.Update(transform, camera);
		
		
		

		for (Layer* layer : m_Layers)
		{
			layer->OnUpdate();
		}
		
		frames ++;
		if (time.Elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			LOG_INFO("FPS: %d", frames);
			frames = 0;
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
	}
}

