#include "Graphics/Window.h"
#include "Layer.h"
#include "Dot/Graphics/Camera.h"

#include "Dot/Graphics/Shader.h"
#include "Dot/Graphics/Transform.h"
#include <vector>


#pragma once
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


	static Application  *Create() { return new Application(); }

private:
	std::unique_ptr<Window> m_Window;

	std::vector<Layer*> m_Layers;
	unsigned int m_LayerInsertIndex = 0;
	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;

	static Application* s_Instance;
	
};

// To be defined in CLIENT
Application *CreateApplication();