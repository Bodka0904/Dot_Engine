#include "Window.h"
#include "Layer.h"
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
	bool m_Running = true;

	std::vector<Layer*> m_Layers;
	unsigned int m_LayerInsertIndex = 0;

};

// To be defined in CLIENT
Application* CreateApplication();