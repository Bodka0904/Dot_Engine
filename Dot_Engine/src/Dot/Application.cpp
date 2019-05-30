#include "stdafx.h"
#include "Application.h"



Application* Application::s_Instance = nullptr;

Application::Application()
	:
	texture(1)
{
	s_Instance = this;
	m_Window = std::unique_ptr<Window>(Window::Create());
	test = GuiWindow::CreateWin(glm::vec2(1, 1), glm::vec3(1, 1, 1));
	test->AddButton(glm::vec2(0.2, 0.2), glm::vec3(0.8, 0.2, 0.8));
	
	guiShader.Init("res/shaders/GuiShader");
	guiShader.Bind();
	//shader.Init("res/shaders/BasicShader");
	//shader.Bind()
	
	mesh = Mesh::CreateMesh("res/models/test.obj");
	texture.Create("res/textures/robotTex.jpg");
	texture.Bind(0);

	test->Init();

	test->GetTranslation().x -= 0.5;
	guiShader.Update(test->GetTranslation());
	
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
	
		
		//transform.GetPos().z = -100;
		//transform.GetRot().y += 0.01;
	
		
		//mesh->Draw();
		//shader.Update(transform, camera);
		
		
		test->Draw();
		
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

