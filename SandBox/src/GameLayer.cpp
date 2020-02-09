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
	physicsSignature.set(Dot::ECSManager::Get()->GetComponentType< Dot::Transform>());
	physicsSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::RigidBody>());
	m_PhysicsSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::PhysicsSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::PhysicsSystem>(physicsSignature);

	Dot::Signature renderSignature;
	renderSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::RenderComponent>());
	renderSignature.set(Dot::ECSManager::Get()->GetComponentType <Dot::Transform>());
	m_RenderSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::RenderSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::RenderSystem>(renderSignature);

	Dot::Signature particleSignature;
	particleSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::Transform>());
	particleSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::ComputeComponent>());
	particleSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::RenderComponent>());
	particleSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::ParticleEmitter>());
	m_ParticleSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::ParticleSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::ParticleSystem>(particleSignature);
	
	Dot::Signature animationSignature;
	animationSignature.set(Dot::ECSManager::Get()->GetComponentType<Dot::AnimationComponent>());
	m_AnimatioSystem = Dot::ECSManager::Get()->RegisterSystem<Dot::AnimationSystem>();
	Dot::ECSManager::Get()->SetSystemSignature<Dot::AnimationSystem>(animationSignature);


	m_Terrain = std::make_shared<Dot::Terrain>(20, 100);
	//m_Terrain = std::make_shared<Dot::Terrain>("res/heightmaps/test.bmp", 100, 2.0f);
	

	m_CamController = std::make_shared<Dot::CameraController>(glm::perspectiveFov(glm::radians(45.0f), float(Dot::Input::GetWindowSize().first) * 0.72f, float(Dot::Input::GetWindowSize().second) * 0.72f, 0.1f, 10000.0f));

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
	m_Tree = Dot::AssetManager::Get()->GetInstancedMesh("InstancedTree");
	m_Tree->models.resize(20);
	for (int i = 0; i < 20; ++i)
	{
		m_Transform.pos = glm::vec3(i*10, 0, i*10);
		m_Transform.UpdateModel();
		m_Tree->models[i] = m_Transform.model;
	}
	m_Tree->UpdateBuffer();
	m_Tree->instances = 20;

	//m_ParticleEffect = std::make_shared<Dot::ParticleEffect>();
	m_ParticleTexture = Dot::AssetManager::Get()->GetTexture("FireTexture");


	
	m_TerrainEntity = Dot::ECSManager::Get()->CreateEntity();

	Dot::ECSManager::Get()->SaveEntity(m_TerrainEntity);

	m_GrassTexure = Dot::Texture2D::Create("res/Textures/Models/grass.png");
	m_Grass = std::make_shared<Dot::BrushInstance>("res/Models/grass.obj", "GrassTexture", 300);
	m_Picker = std::make_shared<Dot::MousePicker>();
	//m_Brush = std::make_shared<Dot::Brush>(20,20,0.2,m_Editor->GetTerrain());
	
	m_Player = std::make_shared<Player>("Man", "ManTexture");

	m_Material = Dot::Material::Create(m_WaterShader);	
	Dot::MaterialStack::Get()->RegisterMaterial("Material",m_Material);
	//m_ParticleEditor = std::make_shared<Dot::ParticleEditor>();

	Dot::Entity entity = Dot::ECSManager::Get()->CreateEntity();
	//m_ParticleEditor->OnCreate(entity);

	//m_RenderableEditor = std::make_shared<Dot::RenderableEditor>();
	//m_RenderableEditor->OnCreate(entity);
	
	m_FrameBuffer = Dot::Framebuffer::Create(512,512,Dot::FramebufferFormat::RGBA16F);

	m_Light = std::make_shared<Dot::Light>();
	m_Light->position = glm::vec3(0.0f, 30.0f, 0.0f);
	m_Light->color = glm::vec3(0.7f, 0.7f, 0.7f);
	m_Light->strength = 1.0f;


	m_WaterTileShader = Dot::Shader::Create("Water Tile Shader", "res/Shaders/Dot/WaterTileShader.glsl");
	m_WaterTile = std::make_shared<Dot::WaterTile>(glm::vec3(0, 0.5, 0), glm::vec2(200, 200));
	m_DuvMap = Dot::Texture2D::Create("res/Textures/Models/dudv.png");
	m_NormalMap = Dot::Texture2D::Create("res/Textures/Models/normalMap.png");


	m_LevelEditor = std::make_shared<Dot::LevelEditor>();



	m_SkyboxEntity = Dot::ECSManager::Get()->CreateEntity();
	m_SkyboxMaterial = std::make_shared<Dot::Material>(Dot::AssetManager::Get()->GetShader("SkyboxShader"));
	m_SkyboxMaterial->Set("u_Texture", Dot::AssetManager::Get()->GetCubeMap("Skybox"));
	Dot::MaterialStack::Get()->RegisterMaterial("Skybox", m_SkyboxMaterial);
	Dot::ECSManager::Get()->AddComponent<Dot::RenderComponent>(m_SkyboxEntity, Dot::RenderComponent{
		std::make_shared<Dot::Skybox>(500.0f),
		Dot::MaterialStack::Get()->GetMaterialID("Skybox"),
		D_QUADS
		});
	Dot::ECSManager::Get()->AddComponent<Dot::Transform>(m_SkyboxEntity, Dot::Transform{});
	Dot::ECSManager::Get()->SaveEntity(m_SkyboxEntity);
}

void GameLayer::OnUpdate(Dot::Timestep ts)
{
	m_LevelEditor->OnRender();
	m_LevelEditor->OnUpdate(ts);

	//m_ParticleEditor->OnUpdate();
	//m_RenderableEditor->OnUpdate();

	m_CollisionSystem->Update();
	m_PhysicsSystem->Update(ts);
	m_ParticleSystem->Update(m_CamController->GetCamera(),ts);
	m_AnimatioSystem->Update(ts);
	//m_Editor->OnUpdate();
	//m_Editor->UpdateBrush(m_CamController->GetCamera());
	
	
	m_CamController->OnUpdate(ts.GetSeconds() * 250);
	m_Player->Update(5, 2, ts.GetSeconds(), *m_Terrain);

	if (m_TimePassed < 1.0f && !m_Swap)
	{
		m_TimePassed += ts;
	}
	else
	{
		m_Swap = true;

	}
	if (m_Swap)
	{
		m_TimePassed -= ts;
		if (m_TimePassed <= -1.0f)
		{
			m_Swap = false;
		}
	}

	//float distance = 2 * (m_CamController->GetCamera().GetPosition().y - m_WaterTile->m_Position.y);
	//glm::vec3 camPos = m_CamController->GetCamera().GetPosition();
	//m_CamController->GetCamera().SetPosition(glm::vec3(camPos.x, camPos.y - distance, camPos.z));
	//m_CamController->GetCamera().InvertPitch();
	//m_CamController->GetCamera().Update();
	//Dot::Renderer::Clear(glm::vec4(1, 1, 1, 0));
	//Dot::Renderer::BeginScene(m_StaticShader,m_CamController->GetCamera());
	//{	
	//	m_FrameBuffer->Bind();
	//	Dot::Renderer::Clear(glm::vec4(1, 1, 1, 0));
	//	
	//	// Must be rendered first to create background for transparent parts
	//	m_SkyBox->GetTexture()->Bind(0);
	//	Dot::Renderer::SubmitElements(m_SkyShader, m_Light, m_SkyBox->GetVAO(), glm::mat4(1), D_QUADS);
	//
	//	//m_WaterShader->Bind();
	//	//m_Material->Set("u_Time", m_TimePassed);
	//	//m_Material->Bind();
	//	//m_Material->Update();
	//	//m_Water->Render(m_WaterShader);
	//		
	//	m_RenderSystem->BeginScene(m_CamController->GetCamera(), m_Light);
	//	{
	//		m_RenderSystem->Render();
	//	}
	//	m_RenderSystem->EndScene(m_StaticShader);
	//	
	//	Dot::RenderCommand::SetBlendFunc(D_SRC_ALPHA, D_ONE_MINUS_SRC_ALPHA);
	//	m_TreeTexture->Bind(0);
	//	m_InstanceShader->Bind();
	//	m_InstanceShader->UploadUniformFloat4("u_ClipPlane", glm::vec4(0, 1, 0, -m_WaterTile->m_Position.y));
	//	m_Tree->Render(m_InstanceShader,D_TRIANGLES);
	//	
	//	//m_StaticShader->Bind();
	//	//m_StaticShader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1));
	//	//m_Terrain->Render(m_StaticShader, D_TRIANGLES);
	//
	//	m_GrassShader->Bind();
	//	m_GrassTexure->Bind(0);
	//	m_Transform.FaceCamera(m_CamController->GetCamera());
	//	m_GrassShader->UploadUniformFloat("u_Time", m_TimePassed);
	//	for (int i = 0; i < m_Grass->GetInstances().size(); ++i)
	//	{	
	//		m_Grass->GetInstances()[i]->Render(m_GrassShader,D_TRIANGLES);
	//	}
	//	
	//	m_FrameBuffer->Unbind();
	//	
	//}
	//Dot::Renderer::EndScene(m_StaticShader);
	//
	//m_CamController->GetCamera().SetPosition(glm::vec3(camPos.x, camPos.y + distance, camPos.z));
	//m_CamController->GetCamera().InvertPitch();
	//m_CamController->GetCamera().Update();
	//Dot::Renderer::BeginScene(m_StaticShader,m_CamController->GetCamera());
	//{
	//	//example->GetWindow("WINDOW2")->GetFBO()->Bind();
	//	Dot::Renderer::Clear(glm::vec4(1, 1, 1, 0));
	//	
	//	
	//	m_SkyBox->GetTexture()->Bind(0);
	//	Dot::Renderer::SubmitElements(m_SkyShader, m_Light, m_SkyBox->GetVAO(), glm::mat4(1), D_QUADS);
	//	
	//
	//	m_RenderSystem->BeginScene(m_CamController->GetCamera(), m_Light);
	//	{
	//		m_RenderSystem->Render();
	//	}
	//	m_RenderSystem->EndScene(m_StaticShader);
	//
	//	Dot::RenderCommand::SetBlendFunc(D_SRC_ALPHA, D_ONE_MINUS_SRC_ALPHA);
	//	//m_TreeTexture->Bind(0);
	//	//m_InstanceShader->Bind();
	//	//m_InstanceShader->UploadUniformFloat4("u_ClipPlane", glm::vec4(0, -1, 0, 1000));
	//	//m_Tree->Render(m_InstanceShader, D_TRIANGLES);
	//
	//	m_DuvMap->Bind(0);
	//	m_NormalMap->Bind(1);
	//	m_FrameBuffer->BindTexture(2);
	//	m_WaterTileShader->Bind();
	//	m_WaterTileShader->UploadUniformFloat("u_Time", m_TimePassed);
	//	//m_WaterTile->Draw();
	//
	//	//example->GetWindow("WINDOW2")->GetFBO()->Unbind();
	//}
	//Dot::Renderer::EndScene(m_StaticShader);

}

void GameLayer::OnEvent(Dot::Event& event)
{
	//m_ParticleEditor->OnEvent(event);
	//m_RenderableEditor->OnEvent(event);
	m_CamController->OnEvent(event);

	if (event.GetEventType() == Dot::EventType::MouseButtonPressed)
	{
		Dot::MouseButtonPressEvent& e = (Dot::MouseButtonPressEvent&)event;

		if (Dot::Input::IsKeyPressed(Dot::D_KEY_LEFT_CONTROL))
		{
			if (e.GetButton() == Dot::D_MOUSE_BUTTON_LEFT)
			{
				m_Picker->CalculateMouseRay(m_CamController->GetCamera());
				//glm::vec3 pos = m_Picker->TestIntersectionTerr(m_CamController->GetCamera(), m_Editor->GetTerrain());
				//m_Brush->SetPosition(pos);
				//m_Brush->Place(m_Grass);
			}
		}
	}

	//m_Editor->OnEvent(event);
}

void GameLayer::OnDetach()
{
}


