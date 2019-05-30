#include "Graphics/Window.h"
#include "Layer.h"
#include "Graphics/Camera.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Graphics/Transform.h"
#include "Graphics/Texture.h"
#include "Events/Event.h"
#include "Dot_GUI/src/GuiWindow.h"
#include "Dot_GUI/src/GuiButton.h"
#include "Dot_GUI/src/shaders/GuiShader.h"





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
	void OnEvent(Event &e);
	
	

	Window GetWin() { return *m_Window; }

	inline static Application& Get() { return *s_Instance; }
	static Application *Create() { return new Application(); }

private:
	std::unique_ptr<Window> m_Window;
	std::vector<Layer*> m_Layers;
	
	GuiWindow * test;
	GuiShader guiShader;

	unsigned int m_LayerInsertIndex = 0;
	unsigned int m_VertexArray;
	unsigned int m_VertexBuffer;
	unsigned int m_IndexBuffer;

	static Application* s_Instance;

	Shader shader;
	Camera camera;
	Transform transform;
	Texture texture;
	Mesh *mesh;
	
};

// To be defined in CLIENT
Application *CreateApplication();