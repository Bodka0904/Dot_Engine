#include "GameLayer.h"

#include <random>
#include <iostream>


void GameLayer::ExampleGuiBlock::OnAttach()
{
	glm::vec2 winSize = Dot::Input::GetWindowSize();
	Dot::Layout layout{
		{glm::vec2(0,0),glm::vec2(0.2,1),{
			{Dot::ElementType::PANEL,0.3,"Panel"},
			{Dot::ElementType::PANEL,0.3,"Panel1"},
			{Dot::ElementType::PANEL,0.4,"Panel2"}
		}},
		{glm::vec2(0.2,0),glm::vec2(0.6,1),{
			{Dot::ElementType::WINDOW,0.6,"WINDOW"},
			{Dot::ElementType::PANEL,0.1,"Panel5"},
			{Dot::ElementType::CONSOLE,0.3,"Console"},
		}},
		{glm::vec2(0.8,0),glm::vec2(0.2,1),{
			{Dot::ElementType::PANEL,0.5,"Panel3"},
			{Dot::ElementType::PANEL,0.5,"Panel4"},
		}}
		
	};

	SetLayout(layout);
	GetPanel("Panel")->AddWidget("Button1", Dot::Button::Create("test", glm::vec2(0), glm::vec2(50, 50), glm::vec3(1, 1, 1)));
	GetPanel("Panel")->AddWidget("Button2", Dot::Button::Create("test", glm::vec2(0), glm::vec2(50, 50), glm::vec3(1, 1, 1)));
	GetPanel("Panel")->AddWidget("Button3", Dot::Button::Create("test", glm::vec2(0), glm::vec2(50, 50), glm::vec3(1, 1, 1)));
	GetPanel("Panel")->AddWidget("Slider", Dot::Slider::Create("slider", glm::vec2(0), glm::vec2(200, 30), glm::vec3(1, 1, 1), &m_Value, -10, 20));	
	GetPanel("Panel")->AddWidget("Checkbox", Dot::CheckBox::Create("checkbox", glm::vec2(0), glm::vec2(50, 50), glm::vec3(1, 1, 1)));
	GetPanel("Panel")->AddWidget("TextArea", Dot::TextArea::Create("textarea", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	GetPanel("Panel")->AddWidget("TextArea1", Dot::TextArea::Create("textarea1", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	GetPanel("Panel")->AddWidget("TextArea2", Dot::TextArea::Create("textarea2", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	GetPanel("Panel")->AddWidget("TextArea3", Dot::TextArea::Create("textarea3", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	
	//GetPanel("Panel1")->AddWidget("TextArea3", Dot::TextArea::Create("textarea3", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	//GetPanel("Panel1")->AddWidget("TextArea4", Dot::TextArea::Create("textarea3", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
	//GetPanel("Panel1")->AddWidget("ArrButton", Dot::ArrowButton::Create("arrbutton", glm::vec2(0), glm::vec2(50, 50), glm::vec3(1, 1, 1)));
	//GetPanel("Panel1")->AddWidget("ArrButton2", Dot::ArrowButton::Create("arrbutton", glm::vec2(0), glm::vec2(50, 50), glm::vec3(1, 1, 1)));

	Dot::Logger::Get()->ConnectConsole(GetConsole("Console"));
	
	//AddWindow("Test Window", Dot::GuiWindow::Create(glm::vec2(400, 0), glm::vec2(1100, 600), glm::vec3(0, 7, 2), "Test Window"));
}

void GameLayer::ExampleGuiBlock::OnDetach()
{

}

void GameLayer::ExampleGuiBlock::OnUpdate()
{
	//if (GetPanel("Panel1")->GetWidget<Dot::ArrowButton>("ArrButton").LeftClicked())
	//{
	//	std::cout << "Left Clicked" << std::endl;
	//}
	//else if (GetPanel("Panel1")->GetWidget<Dot::ArrowButton>("ArrButton").RightClicked())
	//{
	//	std::cout << "Right Clicked" << std::endl;
	//}
	//if (GetPanel("Panel")->GetWidget<Dot::Button>("Button1").Clicked())
	//{
	//	std::cout << "Clicked" << std::endl;
	//}
	//if (GetPanel("Panel")->GetWidget<Dot::CheckBox>("Checkbox").Clicked())
	//{
	//	std::cout << "Checked" << std::endl;
	//}
	//GetPanel("Panel")->GetWidget<Dot::Slider>("Slider").Active(glm::vec2(Dot::Input::GetMousePosition().first, Dot::Input::GetMousePosition().second));
}

void GameLayer::ExampleGuiBlock::OnEvent(Dot::Event& event)
{
	if (event.GetEventType() == Dot::EventType::KeyPressed)
	{
		Dot::KeyPressedEvent& e = (Dot::KeyPressedEvent&)event;
		GetPanel("Panel")->GetWidget<Dot::TextArea>("TextArea").TakeInput(e, Dot::TextArea::TYPE::INT);		
	}
	//if (event.GetEventType() == Dot::EventType::KeyPressed)
	//{
	//	Dot::KeyPressedEvent& e = (Dot::KeyPressedEvent&)event;
	//	GetPanel("Panel1")->GetWidget<Dot::TextArea>("TextArea3").TakeInput(e, Dot::TextArea::TYPE::INT);
	//}
}




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
	
	
	std::vector<std::string> faces{
			"res/textures/Skybox/right.png",
			"res/textures/Skybox/left.png",
			"res/textures/Skybox/top.png",
			"res/textures/Skybox/bottom.png",
			"res/textures/Skybox/back.png",
			"res/textures/Skybox/front.png"
	};
	m_SkyBox = std::make_shared<Dot::Skybox>(faces, 500.0f);

	m_Terrain = std::make_shared<Dot::Terrain>(1, 100);
	//m_Terrain = std::make_shared<Dot::Terrain>("res/heightmaps/test.bmp", 100, 2.0f);
	

	m_CamController = std::make_shared<Dot::CameraController>(glm::perspectiveFov(glm::radians(45.0f), Dot::Input::GetWindowSize().x / 2.0f, Dot::Input::GetWindowSize().y / 2.0f, 0.1f, 10000.0f));

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
	
	m_TerrainEntity = Dot::ECSManager::Get()->CreateEntity();
	//m_Editor = std::make_shared<Dot::TerrainEditor>();
	//m_Editor->OnCreate(m_TerrainEntity);
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
	
	m_FrameBuffer = Dot::Framebuffer::Create(1024,720,Dot::FramebufferFormat::RGBA16F);



	example = new ExampleGuiBlock;
	Dot::GuiApplication::Get()->PushBlock(example);
}

void GameLayer::OnUpdate(Dot::Timestep ts)
{
	example->OnUpdate();

	//m_ParticleEditor->OnUpdate();
	//m_RenderableEditor->OnUpdate();

	m_CollisionSystem->Update();
	m_PhysicsSystem->Update(ts);
	m_ParticleSystem->Update(m_CamController->GetCamera(),ts);
	
	//m_Editor->OnUpdate();
	//m_Editor->UpdateBrush(m_CamController->GetCamera());
	
	
	m_CamController->OnUpdate(ts.GetSeconds() * 250);
	m_LightController->Update(ts);

	
	Dot::Renderer::Clear(glm::vec4(1, 1, 1, 0));
	Dot::Renderer::BeginScene(m_CamController->GetCamera());
	{	
		example->GetWindow("WINDOW")->GetFBO()->Bind();


		Dot::Renderer::Clear(glm::vec4(1, 1, 1, 0));
		// Must be rendered first to create background for transparent parts
		m_SkyBox->GetTexture()->Bind(0);
		Dot::Renderer::SubmitElements(m_SkyShader, m_LightController->GetLight(), m_SkyBox->GetVAO(), glm::mat4(1), D_QUADS);
		
		if (m_TimePassed >= glm::pi<float>())
		{
			m_Swap = true;
		}
		if (m_TimePassed <= -glm::pi<float>())
		{
			m_Swap = false;
		}

		if (!m_Swap)
		{
			m_TimePassed += ts.GetSeconds();
		}
		else
		{
			m_TimePassed -= ts.GetSeconds();
		}
		
		
		
		
		m_WaterShader->Bind();
		m_Material->Set("u_Time", m_TimePassed);
		m_Material->Bind();
		m_Material->Update();
		m_WaterShader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1));
		m_Water->Render(m_WaterShader);
		
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
		
		//m_StaticShader->Bind();
		//m_StaticShader->UploadUniformMat4("u_ModelMatrix", glm::mat4(1));
		//m_Terrain->Render(m_StaticShader, D_TRIANGLES);
	
		m_GrassShader->Bind();
		m_GrassTexure->Bind(0);
		m_Transform.FaceCamera(m_CamController->GetCamera());
		m_GrassShader->UploadUniformFloat("u_Time", m_TimePassed);
		for (int i = 0; i < m_Grass->GetInstances().size(); ++i)
		{	
			m_Grass->GetInstances()[i]->Render(m_GrassShader,D_TRIANGLES);
		}
		
		m_LightShader->Bind();
		Dot::Renderer::SubmitElements(m_LightShader, m_LightController->GetLight(), m_LightController->GetVAO(),m_LightController->GetModel(), D_TRIANGLES);
	}
	Dot::Renderer::EndScene(m_StaticShader);
	example->GetWindow("WINDOW")->GetFBO()->Unbind();
}

void GameLayer::OnEvent(Dot::Event& event)
{
	//m_ParticleEditor->OnEvent(event);
	//m_RenderableEditor->OnEvent(event);
	m_CamController->OnEvent(event);

	if (event.GetEventType() == Dot::EventType::MouseButtonPressed)
	{
		Dot::MouseButtonPressEvent& e = (Dot::MouseButtonPressEvent&)event;

		if (Dot::Input::IsKeyPressed(D_KEY_LEFT_CONTROL))
		{
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
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

