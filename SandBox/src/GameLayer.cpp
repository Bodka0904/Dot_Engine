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

	//m_Terrain = std::make_shared<Dot::Terrain>(500, 100);
	m_Terrain = std::make_shared<Dot::Terrain>("res/heightmaps/test.bmp", 500, 40);
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

	m_TreeTexture = Dot::Texture2D::Create("res/textures/Models/tree.png", true);
	m_TerrTexture = Dot::Texture2D::Create("res/textures/Models/terrain.jpg", true);

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


	m_Transform.m_Rot = glm::vec3(0, 0, 0);
	m_Transform.m_Scale = glm::vec3(5, 5, 5);
	m_TreeTransformations.resize(20);
	for (int i = 0; i < 20; ++i)
	{

		float x = Dot::RandomGenerator::Random(0, m_Terrain->GetSize());
		float z = Dot::RandomGenerator::Random(0, m_Terrain->GetSize());
		
		m_Transform.m_Pos = glm::vec3(x, m_Terrain->GetHeight(glm::vec3(x, 0, z)), z);
		m_TreeTransformations[i] = m_Transform.GetModel();
	}

	m_ParticleShader = Dot::AssetManager::Get()->GetShader("ParticleShader");
	m_ParticleComputeShader = Dot::AssetManager::Get()->GetShader("ParticleComputeShader");
	m_ParticleShader->AddUniform("u_ModelMatrix");
	m_ParticleShader->AddUniform("u_Size");

	m_ParticleComputeShader->AddUniform("u_Time");
	m_ParticleComputeShader->AddUniform("u_Speed");

	m_Tree = Dot::AssetManager::Get()->GetInstancedMesh("InstancedTree");
	m_Tree->Update(m_TreeTransformations, m_TreeTransformations.size(), 0);

	

	m_LightController = std::make_shared<Dot::LightController>(glm::vec3(0, 10, 0), glm::vec3(0.7, 0.7, 0.7));
	m_LightShader = Dot::AssetManager::Get()->GetShader("LightShader");
	m_LightShader->AddUniform("u_ModelMatrix");
	
	m_ParticleEngine = std::make_shared<Dot::ParticleEngine>(128, glm::vec3(0, 0, 0));
	m_ParticleTexture = Dot::AssetManager::Get()->GetTexture("FireTexture");

	m_Water = std::make_shared<Dot::Water>(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec2(10, 10), 50);
	m_Editor = std::make_shared<Dot::TerrainEditor>();


	m_GrassTexure = Dot::Texture2D::Create("res/Textures/Models/grass.png");
	m_Brush = std::make_shared<Dot::Brush>(100, 20, 2.0f);
	m_Grass = std::make_shared<Dot::BrushInstance>("res/Models/grass.obj", "GrassTexture", 300);
	m_Picker = std::make_shared<Dot::MousePicker>();




	Dot::ECSManager::Get()->Init();
	Dot::ECSManager::Get()->RegisterComponent<Dot::AABB>();
	Dot::ECSManager::Get()->RegisterComponent<Dot::RigidBody>();
	Dot::ECSManager::Get()->RegisterComponent<Dot::Transform>();
	
	Dot::Signature signature;
	signature.set(Dot::ECSManager::Get()->GetComponentType<Dot::AABB>());
	signature.set(Dot::ECSManager::Get()->GetComponentType<Dot::RigidBody>());
	m_CollisionSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::CollisionSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::CollisionSystem>(signature);
	

	Dot::Signature signature2;
	signature2.set(Dot::ECSManager::Get()->GetComponentType<Dot::Transform>());
	signature2.set(Dot::ECSManager::Get()->GetComponentType<Dot::RigidBody>());
	m_PhysicsSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::PhysicsSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::PhysicsSystem>(signature2);
	
	
	Dot::Entity entity = Dot::ECSManager::Get()->CreateEntity();
	Dot::ECSManager::Get()->AddComponent(entity, Dot::AABB{ 1,1,1,2,221 });
	Dot::ECSManager::Get()->AddComponent(entity, Dot::RigidBody{ glm::vec3(0,1,0) });

	Dot::Entity entity2 = Dot::ECSManager::Get()->CreateEntity();
	Dot::ECSManager::Get()->AddComponent(entity2, Dot::AABB{ 1,1,1,5,5,5 });
	Dot::ECSManager::Get()->AddComponent(entity2, Dot::RigidBody{ glm::vec3(0,1,0) });

	m_Player = std::make_shared<Player>("Man", "ManTexture");
	
}

void GameLayer::OnUpdate(Dot::Timestep ts)
{
	m_CollisionSystem->Update(ts);
	m_PhysicsSystem->Update(ts);
	m_ParticleEngine->SwapBuffers();


	m_CamController->OnUpdate(ts.GetSeconds() * 250);
	m_LightController->Update(ts);
	m_Editor->UpdateTerrain(m_Terrain);



	Dot::Renderer::Clear(glm::vec4(1, 1, 1, 0.0));	
	Dot::Renderer::BeginScene(m_CamController->GetCamera());
	{	
		
		m_ParticleComputeShader->Compute(32, 32, 1);
		m_ParticleComputeShader->UploadUniformFloat("u_Time", ts);
		m_ParticleComputeShader->UploadUniformFloat("u_Speed", 0.5);

		m_ParticleShader->Bind();
		m_ParticleShader->UploadUniformFloat("u_Size", 0.5);
		m_ParticleTexture->Bind(0);
		
		Dot::Renderer::SubmitArraysCount(m_ParticleShader, m_ParticleEngine->GetVAO(), glm::mat4(1),m_ParticleEngine->GetNumParticles(), D_POINTS);
		

		m_SkyBox->GetTexture()->Bind(0);
		Dot::Renderer::SubmitArrays(m_SkyShader, m_SkyBox->GetVao(), glm::mat4(1), D_TRIANGLES);
	
		m_Player->Update(5, 2, ts.GetSeconds(), *m_LightController->GetLight(), *m_Terrain);
		m_Player->Render();
	
		m_TreeTexture->Bind(0);
		m_InstanceShader->Bind();
		Dot::Renderer::SubmitInstances(m_InstanceShader, m_LightController->GetLight(), m_Tree->GetMesh()->GetVAO(), m_TreeTransformations.size(), D_TRIANGLES);
	
		m_TerrTexture->Bind(0);
		m_StaticShader->Bind();
		Dot::Renderer::SubmitElements(m_StaticShader, m_LightController->GetLight(), m_Terrain->GetVao(), glm::mat4(1), D_TRIANGLES);
	
		m_TimePassed += ts.GetSeconds();
	
		if (m_TimePassed >= glm::pi<float>())
		{
			m_TimePassed = 0;
		}
		m_WaterShader->Bind();
		m_WaterShader->UploadUniformFloat("u_Time", m_TimePassed);
		Dot::Renderer::SubmitElements(m_WaterShader, m_LightController->GetLight(), m_Water->GetVAO(), glm::mat4(1), D_TRIANGLES);
	
	
		m_GrassShader->Bind();
		m_GrassTexure->Bind(0);
		m_GrassShader->UploadUniformFloat("u_Time", m_TimePassed);
		for (int i = 0; i < m_Grass->GetInstances().size(); ++i)
		{
			Dot::Renderer::SubmitInstances(m_GrassShader, m_LightController->GetLight(), m_Grass->GetInstances()[i]->GetMesh()->GetVAO(), m_Grass->GetInstances()[i]->GetNumInstance(), D_TRIANGLES);
		}
	
		m_LightShader->Bind();
		Dot::Renderer::SubmitElements(m_LightShader, m_LightController->GetLight(), m_LightController->GetVAO(),m_LightController->GetModel(), D_TRIANGLES);
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
				float height = m_Terrain->GetHeight(m_Picker->GetRay());
				if (m_Picker->TestIntersection(m_CamController->GetCamera(), pos,height))
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

