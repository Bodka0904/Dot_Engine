#pragma once
#include <Dot.h>

#include "Species/Ant/Colony.h"

class EnvironmentLayer : public Dot::Layer
{
public:
	EnvironmentLayer();
	

	virtual void OnAttach() override;
	virtual void OnUpdate(Dot::Timestep ts) override;
	virtual void OnEvent(Dot::Event& event) override;
	virtual void OnDetach() override;


private:
	Dot::Ref<Dot::Shader> m_ModelShader;
	Dot::Ref <Dot::Shader> m_SkyShader;
	Dot::Ref<Dot::Shader> m_InstanceShader;

	Dot::Ref<Dot::Texture> m_TreeTexture;
	Dot::Ref<Dot::Texture> m_TerrTexture;
	Dot::Ref<Dot::Texture> m_AntTexture;

	Dot::Ref<Dot::InstancedMesh> m_Tree;
	Dot::Ref<Colony> m_Colony;

	Dot::Ref<Dot::Skybox> m_SkyBox;
	Dot::Ref<Dot::Terrain> m_Terrain;
	Dot::Ref<Dot::Camera> m_Camera;


private:
	std::vector<glm::mat4> m_TreeTransformations;

};



class SandBox : public Dot::Application
{
public:
	SandBox()
	{
		PushLayer(new EnvironmentLayer());

	}

	~SandBox()
	{

	}

private:

};




Dot::Application* CreateApplication()
{
	return new SandBox();
}

