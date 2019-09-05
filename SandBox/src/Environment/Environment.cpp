#include "Environment.h"

#include <random>



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
	m_Terrain->ApplyHeightsValueNoise(5);
	m_Terrain->ApplyNormals();
	

	m_Camera = std::make_shared<Dot::Camera>(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f));


	Dot::ShaderLayout sm_layout = {
			{0,"position"},
			{1,"normal"},
			{2,"tangent"},
			{3,"binormal"},
			{4,"texCoord"},

	};
	m_ModelShader = std::make_shared<Dot::Shader>("res/shaders/Dot/BasicShader.vs", "res/shaders/Dot/BasicShader.fs");
	m_ModelShader->SetLayout(sm_layout);
	m_ModelShader->LinkShader();

	m_ModelShader->Bind();
	m_ModelShader->AddUniform("u_Texture");
	m_ModelShader->UploadInt("u_Texture", 0);
	m_ModelShader->SetUniforms();
	m_ModelShader->AddUniformBufferObject("camera_data", 0, 3 * sizeof(glm::mat4));
	m_ModelShader->AddUniform("ModelMatrix");

	Dot::ShaderLayout si_layout = {
			{0,"position"},
			{1,"normal"},
			{4,"texCoord"},
			{3,"instanceModel"},
	};

	m_TreeTexture = std::make_shared<Dot::Texture>("res/projectmodels/tree.png");
	m_TerrTexture = std::make_shared<Dot::Texture>("res/textures/Dot/grass.jpg");

	m_InstanceShader = std::make_shared<Dot::Shader>("res/shaders/Dot/InstancedShader.vs", "res/shaders/Dot/InstancedShader.fs");
	m_InstanceShader->SetLayout(si_layout);
	m_InstanceShader->LinkShader();
	m_InstanceShader->Bind();

	m_InstanceShader->AddUniform("u_Texture");
	m_InstanceShader->UploadInt("u_Texture", 0);

	Dot::ShaderLayout ss_layout = {
		   {0,"position"}
	};

	m_SkyShader = std::make_shared<Dot::Shader>("res/shaders/Dot/SkyboxShader.vs", "res/shaders/Dot/SkyboxShader.fs");
	m_SkyShader->SetLayout(ss_layout);
	m_SkyShader->LinkShader();


	

	m_TreeTransformations.resize(100);
	for (int i = 0; i < 100; ++i)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<int> dist(0, m_Terrain->GetSize());
		
		float x = dist(eng);
		float z = dist(eng);

		glm::mat4 tmp = glm::mat4(1);
		tmp = glm::translate(glm::vec3(x, m_Terrain->GetHeight(glm::vec3(x, 0, z)), z));

		m_TreeTransformations[i] = tmp;
	}

	 m_Tree = std::make_shared<Dot::InstancedMesh>("res/projectmodels/tree.obj",m_TreeTransformations);

	 m_AntTexture = std::make_shared<Dot::Texture>("res/projectmodels/ant.png");
	 m_Colony = std::make_shared<Colony>("res/projectmodels/ant.obj");
}

void EnvironmentLayer::OnUpdate(Dot::Timestep ts)
{
	Dot::Renderer::Clear(glm::vec4(0.4, 0.5, 0.7, 0.0));
	
	m_Camera->Update(ts.GetSeconds()*250);
	m_Colony->Update();

	Dot::Renderer::BeginScene(*m_Camera);
	{
		m_SkyBox->GetTexture()->Bind(0);
		Dot::Renderer::SubmitArrays(m_SkyShader, m_SkyBox->GetVao(), glm::mat4(1), D_TRIANGLES);

		m_TreeTexture->Bind(0);
		Dot::Renderer::SubmitInstances(m_InstanceShader, m_Tree,m_TreeTransformations.size(), D_TRIANGLES);

		m_TerrTexture->Bind(0);
		Dot::Renderer::SubmitElementsVao(m_ModelShader, m_Terrain->GetVao(), glm::mat4(1), D_TRIANGLES);

		m_AntTexture->Bind(0);
		Dot::Renderer::SubmitInstances(m_InstanceShader, m_Colony->GetModel(),m_Colony->GetSize(), D_TRIANGLES);
	}
	Dot::Renderer::EndScene(m_ModelShader);
}

void EnvironmentLayer::OnEvent(Dot::Event& event)
{
}

void EnvironmentLayer::OnDetach()
{
}
