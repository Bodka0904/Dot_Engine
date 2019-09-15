#include "Environment.h"

#include <random>
#include <iostream>


EnvironmentLayer::EnvironmentLayer()
	: Dot::Layer()
{
}

void EnvironmentLayer::OnAttach()
{


	std::vector<std::string> faces{
			"res/textures/skybox/test/right.png",
			"res/textures/skybox/test/left.png",
			"res/textures/skybox/test/top.png",
			"res/textures/skybox/test/bottom.png",
			"res/textures/skybox/test/back.png",
			"res/textures/skybox/test/front.png"
	};
	m_SkyBox = std::make_shared<Dot::Skybox>(faces, 500);
	
	m_Terrain = std::make_shared<Dot::Terrain>(200, 50);
	//m_Terrain->ApplyHeightsValueNoise(5);
	//m_Terrain->ApplyNormals();
	

	m_Camera = std::make_shared<Dot::Camera>(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f));


	Dot::ShaderLayout sm_layout = {
			{0,"position"},
			{1,"normal"},
			{2,"tangent"},
			{3,"binormal"},
			{4,"texCoord"},

	};
	m_StaticShader = std::make_shared<Dot::Shader>("TEST","res/shaders/Dot/StaticShader.glsl");
	m_StaticShader->Bind();
	m_StaticShader->AddUniform("u_Texture");
	m_StaticShader->UploadUniformInt("u_Texture", 0);
	
	m_StaticShader->AddUniformBufferObject("camera_data", 0, 3 * sizeof(glm::mat4));
	m_StaticShader->AddUniform("u_ModelMatrix");

	Dot::ShaderLayout si_layout = {
			{0,"position"},
			{1,"normal"},
			{4,"texCoord"},
			{3,"instanceModel"},
	};

	m_TreeTexture = std::make_shared<Dot::Texture>("res/projectmodels/tree.png");
	m_TerrTexture = std::make_shared<Dot::Texture>("res/textures/Dot/grass.jpg");

	m_InstanceShader = std::make_shared<Dot::Shader>("InstanceShader", "res/shaders/Dot/InstanceShader.glsl");
	m_InstanceShader->Bind();

	m_InstanceShader->AddUniform("u_Texture");
	m_InstanceShader->UploadUniformInt("u_Texture", 0);

	Dot::ShaderLayout ss_layout = {
		   {0,"position"}
	};

	m_SkyShader = std::make_shared<Dot::Shader>("SkyboxShader", "res/shaders/Dot/SkyboxShader.glsl");
;

	m_TreeTransformations.resize(20);
	for (int i = 0; i < 20; ++i)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<int> dist(0, m_Terrain->GetSize());
		
		float x = dist(eng);
		float z = dist(eng);
		
		m_T.SetPos(glm::vec3(x, m_Terrain->GetHeight(glm::vec3(x, 0, z)), z));
		m_T.SetScale(glm::vec3(5, 5, 5));
		m_T.UpdateModel();
		m_TreeTransformations[i] = m_T.GetModel();
	}

	m_ShaderForCompute = std::make_shared<Dot::Shader>("TestForCompute","res/shaders/Dot/TestForComputeShader.glsl");
	m_ComputeShader = std::make_shared<Dot::Shader>("BasicComputShader","res/shaders/Dot/ComputeShader.glsl");

	m_Tree = std::make_shared<Dot::InstancedMesh>("res/projectmodels/tree.obj",m_TreeTransformations);
	m_Player = std::make_shared<Player>("res/animation/cowboy.dae", "res/textures/Dot/cowboy.png");


	m_TestManager = std::make_shared<Dot::ParticleManager>(1000);
}

void EnvironmentLayer::OnUpdate(Dot::Timestep ts)
{
	Dot::Renderer::Clear(glm::vec4(0.4, 0.5, 0.7, 0.0));
	
	m_Camera->Update(ts.GetSeconds()*250);

	m_TestManager->Run();
	m_ComputeShader->Compute(32, 32, 1);
	m_ShaderForCompute->Bind();
	m_TestManager->Draw();

	Dot::Renderer::BeginScene(*m_Camera);
	{

		m_Player->Update(0.05,0.01,ts.GetSeconds());
		m_Player->Render();

		m_SkyBox->GetTexture()->Bind(0);
		Dot::Renderer::SubmitArrays(m_SkyShader, m_SkyBox->GetVao(), glm::mat4(1), D_TRIANGLES);

		m_TreeTexture->Bind(0);
		Dot::Renderer::SubmitInstances(m_InstanceShader, m_Tree,m_TreeTransformations.size(), D_TRIANGLES);

		m_TerrTexture->Bind(0);
		Dot::Renderer::SubmitElementsVao(m_StaticShader, m_Terrain->GetVao(), glm::mat4(1), D_TRIANGLES);

	}
	Dot::Renderer::EndScene(m_StaticShader);
}

void EnvironmentLayer::OnEvent(Dot::Event& event)
{
}

void EnvironmentLayer::OnDetach()
{
}
