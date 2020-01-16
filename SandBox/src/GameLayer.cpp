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
	
	
	
	Dot::Signature collisionSignature;
	collisionSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::AABB>());
	collisionSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::RigidBody>());
	m_CollisionSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::CollisionSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::CollisionSystem>(collisionSignature);


	Dot::Signature physicsSignature;
	physicsSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::Transform>());
	physicsSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::RigidBody>());
	m_PhysicsSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::PhysicsSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::PhysicsSystem>(physicsSignature);

	Dot::Signature renderSignature;
	renderSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::RenderComponent>());
	renderSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::Transform>());
	m_RenderSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::RenderSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::RenderSystem>(renderSignature);

	Dot::Signature particleSignature;
	particleSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::Transform>());
	particleSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::RenderComponent>());
	particleSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::ParticleComponent>());
	particleSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::Ref<Dot::ParticleEffect>>());
	m_ParticleSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::ParticleSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::ParticleSystem>(particleSignature);
	
	
	std::vector<std::string> faces{
			"res/textures/Skybox/right.png",
			"res/textures/Skybox/left.png",
			"res/textures/Skybox/top.png",
			"res/textures/Skybox/bottom.png",
			"res/textures/Skybox/back.png",
			"res/textures/Skybox/front.png"
	};
	m_SkyBox = std::make_shared<Dot::Skybox>(faces, 500.0f);

	m_Terrain = std::make_shared<Dot::Terrain>(300, 129);
	//m_Terrain = std::make_shared<Dot::Terrain>("res/heightmaps/test.bmp", 100, 2.0f);
	//m_Terrain->HeightsValueNoise();
	m_Terrain->DiamondSquare();
	m_Terrain->ApplyHeights();
	m_Terrain->ApplyNormals();


	m_CamController = std::make_shared<Dot::CameraController>(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f));

	m_StaticShader = Dot::AssetManager::Get()->GetShader("StaticShader");
	m_InstanceShader = Dot::AssetManager::Get()->GetShader("InstanceShader");
	m_GrassShader = Dot::AssetManager::Get()->GetShader("GrassShader");
	m_SkyShader = Dot::AssetManager::Get()->GetShader("SkyboxShader");
	m_WaterShader = Dot::AssetManager::Get()->GetShader("WaterShader");
	m_LightShader = Dot::AssetManager::Get()->GetShader("LightShader");
	m_ParticleShader = Dot::AssetManager::Get()->GetShader("ParticleShader");
	m_ParticleComputeShader = Dot::AssetManager::Get()->GetShader("ParticleComputeShader");


	m_TreeTexture = Dot::Texture2D::Create("res/textures/Models/tree.png", true);
	m_TerrTexture = Dot::Texture2D::Create("res/textures/Models/terrain.jpg", true);


	m_Transform.rot = glm::vec3(0, 0, 0);
	m_Transform.scale = glm::vec3(5, 5, 5);
	m_TreeTransformations.resize(20);
	for (int i = 0; i < 20; ++i)
	{

		float x = Dot::RandomGenerator::Random(0, m_Terrain->GetSize());
		float z = Dot::RandomGenerator::Random(0, m_Terrain->GetSize());
		
		m_Transform.pos = glm::vec3(x, m_Terrain->GetHeight(glm::vec3(x, 0, z)), z);
		m_Transform.UpdateModel();
		m_TreeTransformations[i] = m_Transform.model;
	}


	m_Tree = Dot::AssetManager::Get()->GetInstancedMesh("InstancedTree");
	m_Tree->Update(m_TreeTransformations, m_TreeTransformations.size(), 0);

	m_LightController = std::make_shared<Dot::LightController>(glm::vec3(0, 200, 0), glm::vec3(0.8, 0.8, 0.8));
	
	
	//m_ParticleEffect = std::make_shared<Dot::ParticleEffect>();
	m_ParticleTexture = Dot::AssetManager::Get()->GetTexture("FireTexture");

	m_Water = std::make_shared<Dot::Water>(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec2(10, 10), 50);
	m_Editor = std::make_shared<Dot::TerrainEditor>();


	m_GrassTexure = Dot::Texture2D::Create("res/Textures/Models/grass.png");
	m_Brush = std::make_shared<Dot::TerrainBrush>(20,m_Terrain);
	m_Grass = std::make_shared<Dot::BrushInstance>("res/Models/grass.obj", "GrassTexture", 300);
	m_Picker = std::make_shared<Dot::MousePicker>();

	
	Dot::Entity entity = Dot::ECSManager::Get()->CreateEntity();
	Dot::ECSManager::Get()->AddComponent(entity, Dot::AABB{ 1,1,1,2,2,2 });
	Dot::ECSManager::Get()->AddComponent(entity, Dot::RigidBody{ glm::vec3(0,1,0) });

	Dot::Entity entity2 = Dot::ECSManager::Get()->CreateEntity();
	Dot::ECSManager::Get()->AddComponent(entity2, Dot::AABB{ 1,1,1,5,5,5 });
	Dot::ECSManager::Get()->AddComponent(entity2, Dot::RigidBody{ glm::vec3(0,1,0) });
	
	m_Player = std::make_shared<Player>("Man", "ManTexture");

	m_Material = Dot::Material::Create(m_WaterShader);	

}

void GameLayer::OnUpdate(Dot::Timestep ts)
{

	m_CollisionSystem->Update();
	m_PhysicsSystem->Update(ts);
	m_ParticleSystem->Update(m_CamController->GetCamera(),ts);
	
	
	m_CamController->OnUpdate(ts.GetSeconds() * 250);
	m_LightController->Update(ts);
	m_Editor->UpdateTerrain(m_Terrain);



	Dot::Renderer::Clear(glm::vec4(1, 1, 1, 0.0));	
	
	Dot::Renderer::BeginScene(m_CamController->GetCamera());
	{	
		// Must be rendered first to create background for transparent parts
		m_SkyBox->GetTexture()->Bind(0);
		Dot::Renderer::SubmitElements(m_SkyShader, m_LightController->GetLight(), m_SkyBox->GetVAO(), glm::mat4(1), D_QUADS);
		
		
		m_Player->Update(5, 2, ts.GetSeconds(), *m_Terrain);
		m_RenderSystem->BeginScene(m_CamController->GetCamera(), m_LightController->GetLight());
		{	
			m_RenderSystem->Render();
		}
		m_RenderSystem->EndScene(m_StaticShader);

		Dot::RenderCommand::SetBlendFunc(D_SRC_ALPHA, D_ONE_MINUS_SRC_ALPHA);
		m_TreeTexture->Bind(0);
		m_InstanceShader->Bind();
		//m_Tree->Render(m_InstanceShader);
		
		m_TerrTexture->Bind(0);
		m_StaticShader->Bind();
		m_StaticShader->UploadUniformFloat("u_Radius", m_Brush->GetRadius());
		m_StaticShader->UploadUniformFloat2("u_BrushPosition",glm::vec2(m_Brush->GetPosition().x,m_Brush->GetPosition().z));
		Dot::Renderer::SubmitElements(m_StaticShader, m_LightController->GetLight(), m_Terrain->GetVAO(), glm::mat4(1), D_TRIANGLES);
		
		m_TimePassed += ts.GetSeconds();
		
		if (m_TimePassed >= glm::pi<float>())
		{
			m_TimePassed = 0;
		}
		m_WaterShader->Bind();
		
		m_Material->Set("u_Time", m_TimePassed);
		m_Material->Bind();
		m_Material->Update();
		m_WaterShader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1));
		//m_Water->Render(m_WaterShader);
		
		
		m_GrassShader->Bind();
		m_GrassTexure->Bind(0);
		m_GrassShader->UploadUniformFloat("u_Time", m_TimePassed);
		for (int i = 0; i < m_Grass->GetInstances().size(); ++i)
		{	
			m_Grass->GetInstances()[i]->Render(m_GrassShader,D_TRIANGLES);
		}
		
		m_LightShader->Bind();
		//Dot::Renderer::SubmitElements(m_LightShader, m_LightController->GetLight(), m_LightController->GetVAO(),m_LightController->GetModel(), D_TRIANGLES);
	}
	Dot::Renderer::EndScene(m_StaticShader);

	glm::vec3 pos = glm::vec3(0);
	m_Picker->CalculateMouseRay(m_CamController->GetCamera());
	pos = m_Picker->TestIntersectionTerr(m_CamController->GetCamera(), m_Terrain);
	m_Brush->SetPosition(pos);
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
				//m_Brush->Place(m_Grass);
				m_Brush->ApplyBrush();
			}
		}
	}
}

void GameLayer::OnDetach()
{

}

