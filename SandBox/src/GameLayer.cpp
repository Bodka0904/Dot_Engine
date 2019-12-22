#include "GameLayer.h"

#include <random>
#include <iostream>



GameLayer::GameLayer()
	: Dot::Layer()
{
}

void GameLayer::OnAttach()
{
	Dot::Renderer::Init();
	std::vector<std::string> faces{
			"res/textures/Skybox/right.png",
			"res/textures/Skybox/left.png",
			"res/textures/Skybox/top.png",
			"res/textures/Skybox/bottom.png",
			"res/textures/Skybox/back.png",
			"res/textures/Skybox/front.png"
	};
	m_SkyBox = std::make_shared<Dot::Skybox>(faces, 500);

	m_Terrain = std::make_shared<Dot::Terrain>(500,100);
	//m_Terrain = std::make_shared<Dot::Terrain>("res/heightmaps/test.bmp", 500, 40);
	//m_Terrain->ApplyHeightsValueNoise(15.0f);
	//m_Terrain->Update();
	m_Terrain->ApplyNormals();


	m_CamController = std::make_shared<Dot::CameraController>(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f));

	m_StaticShader = Dot::Shader::Create("TEST", "res/Shaders/Dot/StaticShader.glsl");
	m_StaticShader->Bind();
	//m_StaticShader->AddUniform("u_Texture");
	//m_StaticShader->UploadUniformInt("u_Texture", 0);

	m_StaticShader->AddUniformBufferObject("camera_data", 0, (sizeof(glm::mat4) * 3) + sizeof(glm::vec3));
	m_StaticShader->AddUniform("u_ModelMatrix");
	m_StaticShader->AddUniform("u_LightPosition");
	m_StaticShader->AddUniform("u_LightColor");
	m_StaticShader->AddUniform("u_LightStrength");

	m_TreeTexture = Dot::Texture2D::Create("res/textures/Dot/tree.png", true);
	m_TerrTexture = Dot::Texture2D::Create("res/textures/Dot/terrain.jpg", true);

	m_InstanceShader = Dot::Shader::Create("InstanceShader", "res/Shaders/Dot/InstanceShader.glsl");
	m_InstanceShader->Bind();

	//m_InstanceShader->AddUniform("u_Texture");
	//m_InstanceShader->UploadUniformInt("u_Texture", 0);
	m_InstanceShader->AddUniform("u_LightPosition");
	m_InstanceShader->AddUniform("u_LightColor");
	m_InstanceShader->AddUniform("u_LightStrength");

	m_GrassShader = Dot::Shader::Create("GrassShader", "res/Shaders/Dot/GrassShader.glsl");
	m_GrassShader->Bind();
	m_GrassShader->AddUniform("u_LightPosition");
	m_GrassShader->AddUniform("u_LightColor");
	m_GrassShader->AddUniform("u_LightStrength");
	m_GrassShader->AddUniform("u_Time");

	m_SkyShader = Dot::Shader::Create("SkyboxShader", "res/Shaders/Dot/SkyboxShader.glsl");
	m_SkyShader->AddUniform("u_ModelMatrix");

	m_WaterShader = Dot::Shader::Create("WaterShader", "res/Shaders/Dot/WaterShader.glsl");
	m_WaterShader->AddUniform("u_ModelMatrix");
	m_WaterShader->AddUniform("u_Time");
	m_WaterShader->AddUniform("u_LightPosition");
	m_WaterShader->AddUniform("u_LightColor");
	m_WaterShader->AddUniform("u_LightStrength");


	m_TreeTransformations.resize(20);
	for (int i = 0; i < 20; ++i)
	{

		float x = Dot::RandomGenerator::Random(0, m_Terrain->GetSize());
		float z = Dot::RandomGenerator::Random(0, m_Terrain->GetSize());

		m_T.SetPos(glm::vec3(x, m_Terrain->GetHeight(glm::vec3(x, 0, z)), z));
		//m_T.SetPos(glm::vec3(x, 0, z));
		m_T.SetScale(glm::vec3(5, 5, 5));
		m_T.UpdateModel();
		m_TreeTransformations[i] = m_T.GetModel();
	}

	m_ShaderForCompute = Dot::Shader::Create("TestForCompute", "res/Shaders/Dot/TestForComputeShader.glsl");
	m_ComputeShader = Dot::Shader::Create("BasicComputShader", "res/Shaders/Dot/ComputeShader.glsl");

	m_Tree = Dot::AssetManager::Get()->GetInstancedMesh("InstancedTree");
	m_Tree->Update(m_TreeTransformations, m_TreeTransformations.size(), 0);

	m_Player = std::make_shared<Player>("res/Animations/test.fbx", "res/Textures/Dot/man.png");

	m_Light = std::make_shared<Dot::Light>(glm::vec3(-20, 1000, 0), glm::vec3(0.7, 0.7, 0.7));

	m_TestManager = std::make_shared<Dot::ParticleManager>(1000);



	m_Water = std::make_shared<Dot::Water>(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec2(10, 10), 50);
	m_Editor = std::make_shared<Dot::TerrainEditor>();

	Dot::Slider::Create("Light Strength", glm::vec2(50, 50), glm::vec2(200, 20), &m_Light->GetStrength());


	m_GrassTexure = Dot::Texture2D::Create("res/Textures/Dot/grass.png");
	m_Brush = std::make_shared<Dot::Brush>(100, 20, 2.0f);
	m_Grass = std::make_shared<Dot::BrushInstance>("res/Models/grass.obj", "Grass", 300);
	m_Picker = std::make_shared<Dot::MousePicker>();



	// Example of usage of ECSManager //
	struct Position
	{
		Position(){}
		Position(int x,int y)
			:m_x(x),m_y(y)
		{}
		int m_x;
		int m_y;
	};

	m_ECSManager.Init();
	m_ECSManager.RegisterComponent<Position>();
	
	Dot::Signature signature;
	signature.set(m_ECSManager.GetComponentType<Position>());
	

	Dot::Entity entity = m_ECSManager.CreateEntity();
	
	m_ECSManager.AddComponent(entity, Position{ 5,4 });


	Position positon = m_ECSManager.GetComponent<Position>(entity);
	std::cout << positon.m_x << std::endl;
	////////////////////////////////////////////
}

void GameLayer::OnUpdate(Dot::Timestep ts)
{

	Dot::Renderer::Clear(glm::vec4(1, 1, 1, 0.0));
	m_CamController->OnUpdate(ts.GetSeconds() * 250);
	m_TestManager->Run();
	m_ComputeShader->Compute(32, 32, 1);
	m_ShaderForCompute->Bind();
	m_TestManager->Draw();

	m_Editor->UpdateTerrain(m_Terrain);
	Dot::Renderer::BeginScene(m_CamController->GetCamera());
	{
		m_SkyBox->GetTexture()->Bind(0);
		Dot::Renderer::SubmitArrays(m_SkyShader, m_SkyBox->GetVao(), glm::mat4(1), D_TRIANGLES);

		m_Player->Update(5, 2, ts.GetSeconds(), *m_Light, *m_Terrain);
		m_Player->Render();

		m_TreeTexture->Bind(0);
		m_InstanceShader->Bind();
		Dot::Renderer::SubmitInstances(m_InstanceShader, m_Light, m_Tree->GetVAO(), m_TreeTransformations.size(), D_TRIANGLES);

		m_TerrTexture->Bind(0);
		m_StaticShader->Bind();
		Dot::Renderer::SubmitElements(m_StaticShader, m_Light, m_Terrain->GetVao(), glm::mat4(1), D_TRIANGLES);

		m_TimePassed += ts.GetSeconds();

		if (m_TimePassed >= glm::pi<float>())
		{
			m_TimePassed = 0;
		}
		m_WaterShader->Bind();
		m_WaterShader->UploadUniformFloat("u_Time", m_TimePassed);
		//Dot::Renderer::SubmitElements(m_WaterShader,m_Light, m_Water->GetVAO(), glm::mat4(1), D_TRIANGLES);


		m_GrassShader->Bind();
		m_GrassTexure->Bind(0);
		m_GrassShader->UploadUniformFloat("u_Time", m_TimePassed);
		for (int i = 0; i < m_Grass->GetInstances().size(); ++i)
		{
			Dot::Renderer::SubmitInstances(m_GrassShader, m_Light, m_Grass->GetInstances()[i]->GetVAO(), m_Grass->GetInstances()[i]->GetNumInstance(), D_TRIANGLES);
		}

	}
	Dot::Renderer::EndScene(m_StaticShader);
}

void GameLayer::OnEvent(Dot::Event& event)
{
	m_CamController->OnEvent(event);

	if (event.GetEventType() == Dot::EventType::MouseButtonPressed)
	{
		Dot::MouseButtonPressEvent& e = (Dot::MouseButtonPressEvent&)event;

		if (Dot::Input::IsKeyPressed(D_KEY_LEFT_SHIFT))
		{
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{
				m_Picker->CalculateMouseRay(m_CamController->GetCamera());
				glm::vec3 pos;
				if (m_Picker->TestIntersection(*m_Terrain, m_CamController->GetCamera(), pos))
				{
					m_Brush->Place(pos, m_Grass, m_Terrain);
				}
			}
		}
	}
}

void GameLayer::OnDetach()
{
}
