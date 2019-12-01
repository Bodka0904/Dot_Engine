#pragma once
#include <Dot.h>
#include "Player.h"


class Sandbox : public Dot::Layer
{
public:
	Sandbox();
	

	virtual void OnAttach() override;
	virtual void OnUpdate(Dot::Timestep ts) override;
	virtual void OnEvent(Dot::Event& event) override;
	virtual void OnDetach() override;


private:
	Dot::Ref<Dot::Shader> m_StaticShader;
	Dot::Ref <Dot::Shader> m_SkyShader;
	Dot::Ref<Dot::Shader> m_InstanceShader;
	Dot::Ref<Dot::Shader> m_ComputeShader;
	Dot::Ref<Dot::Shader> m_ShaderForCompute;
	Dot::Ref<Dot::Shader> m_WaterShader;

	Dot::Ref<Dot::Texture> m_TreeTexture;
	Dot::Ref<Dot::Texture> m_TerrTexture;
	
	Dot::Ref<Dot::InstancedMesh> m_Tree;
	Dot::Ref<Player> m_Player;

	
	Dot::Ref<Dot::Skybox> m_SkyBox;
	Dot::Ref<Dot::Terrain> m_Terrain;
	Dot::Ref<Dot::TerrainEditor>m_Editor;

	Dot::Ref<Dot::Water>m_Water;
	Dot::Ref<Dot::Camera> m_Camera;
	Dot::Ref<Dot::Light> m_Light;

	Dot::Ref<Dot::ParticleManager> m_TestManager;

private:
	Dot::Transform m_T;
	std::vector<glm::mat4> m_TreeTransformations;

private:
	float m_TimePassed = 0.0f;

};



class SandBox : public Dot::Application
{
public:
	SandBox()
	{
		PushLayer(new Sandbox());

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

