#pragma once
#include <Dot.h>
#include <iostream>
class TestLayer : public Dot::Layer
{
public:
	TestLayer()
		: 
		Dot::Layer(),
		camera(45.0f,1780.0f/1240.0f,1.0f,10000.0f)
	{

	}

	void OnAttach()
	{	

		Dot::ShaderLayout s_layout = {
			{0,"position"},
			{1,"normal"},
			{2,"tangent"},
			{3,"binormal"},
			{4,"texCoord"},
	
		};
		
		//WorldShader = std::make_shared<Dot::Shader>("res/shaders/Dot/TestGeometryShader");
		WorldShader = std::make_shared<Dot::Shader>("res/shaders/Dot/BasicShader.vs","res/shaders/Dot/BasicShader.fs");
		//WorldShader->AddGeometryShader("res/shaders/Dot/GeometryShader.gs");
		WorldShader->SetLayout(s_layout);
		WorldShader->LinkShader();
	
		WorldShader->Bind();
		WorldShader->SetUniforms();
		WorldShader->AddUniformBufferObject("camera_data", 0,3*sizeof(glm::mat4));
		WorldShader->AddUniform("ModelMatrix");
		WorldShader->AddUniform("time");
			
		
		TreeTexture.Create2D("res/textures/Dot/treehill.png", Dot::TextureFormat::RGBA);

		Dot::ShaderLayout test_layout = {
			{0,"position"},
			{1,"normal"},
			{4,"texCoord"},
			{3,"instanceModel"},
		};

		InstanceShader = std::make_shared<Dot::Shader>("res/shaders/Dot/InstancedShader.vs", "res/shaders/Dot/InstancedShader.fs");
		InstanceShader->SetLayout(test_layout);
		InstanceShader->LinkShader();
	
		InstanceShader->Bind();
		InstanceShader->AddUniform("texi");
		InstanceShader->UploadInt("texi", 1);
		
		
		InstancePositions.resize(5);		
		transform.GetPos().x += 30;
		transform.SetScale(glm::vec3(10, 10, 10));
		InstancePositions[0] = transform.GetModel();
		transform.GetPos().x += 30;
		InstancePositions[1] = transform.GetModel();
		transform.GetPos().x += 30;
		InstancePositions[2] = transform.GetModel();
		transform.GetPos().x += 30;
		InstancePositions[3] = transform.GetModel();
		transform.GetPos().x = 0;


		InstanceModel = std::make_shared<Dot::InstancedMesh>("res/models/Dot/treehill.obj", InstancePositions);

		Dot::ShaderLayout skyLayout = {
			{0,"position"}
		};

		SkyBoxShader = std::make_shared<Dot::Shader>("res/shaders/Dot/SkyboxShader.vs", "res/shaders/Dot/SkyboxShader.fs");
		SkyBoxShader->SetLayout(skyLayout);
		SkyBoxShader->LinkShader();
	
		
		std::vector<std::string> faces{ 
			"res/textures/skybox/test/right.png",
			"res/textures/skybox/test/left.png",
			"res/textures/skybox/test/top.png",
			"res/textures/skybox/test/bottom.png",
			"res/textures/skybox/test/back.png",
			"res/textures/skybox/test/front.png"
		};

		SkyBox = std::make_shared<Dot::Skybox>(faces,500);


		TerrTexture.Create2D("res/textures/Dot/grass.jpg",Dot::TextureFormat::RGBA);


		GunTexture.Create2D("res/textures/Dot/dragon.png", Dot::TextureFormat::RGBA);
		Mesh = std::make_shared<Dot::Mesh>("res/models/Dot/dragon.obj");


		Dot::ShaderLayout anim = {
			{0,"Position"},
			{1,"TexCoord"},
			{2,"Normal"},
			{3,"BoneIDs"},
			{4,"Weights"}
		};
		m_Anim_Shader = std::make_shared<Dot::Shader>("res/shaders/Dot/AnimShader.vs", "res/shaders/Dot/AnimShader.fs");
		m_Anim_Shader->SetLayout(anim);
		m_Anim_Shader->LinkShader();


		m_Anim_Test = std::make_shared<Dot::AnimatedMesh>("res/animation/cowboy.dae");
		m_Anim_Shader->AddUniform("ModelMatrix");
		m_Anim_Shader->AddUniform("gBones");
		CowBoyTex.Create2D("res/textures/Dot/cowboy.png", Dot::TextureFormat::RGBA);
	}

	void OnUpdate(Dot::Timestep ts) override
	{
		//LOG_INFO("%f ms %f s", ts.GetMiliseconds(), ts.GetSeconds());
		//CAMERA CONTROL - DEBUG PURPOSE
		
		if (Dot::Input::IsKeyPressed(D_KEY_W))
		{
			camera.GetTarget().y += m_CameraRotationSpeed * ts;
		}
		if (Dot::Input::IsKeyPressed(D_KEY_S))
		{
			camera.GetTarget().y -= m_CameraRotationSpeed * ts;
		}
		if (Dot::Input::IsKeyPressed(D_KEY_A))
		{
			camera.GetRotation().y += m_CameraRotationSpeed * ts;
			camera.GetTarget().x = sin(camera.GetRotation().y);
			camera.GetTarget().z = cos(camera.GetRotation().y);
		}
		if (Dot::Input::IsKeyPressed(D_KEY_D))
		{
			camera.GetRotation().y -= m_CameraRotationSpeed * ts;
			camera.GetTarget().x = sin(camera.GetRotation().y);
			camera.GetTarget().z = cos(camera.GetRotation().y);
		}
		if (Dot::Input::IsKeyPressed(D_KEY_UP))
		{
			camera.GetPosition() += glm::vec3(m_CameraMoveSpeed * ts, m_CameraMoveSpeed * ts, m_CameraMoveSpeed * ts) * camera.GetTarget();
		}
		if (Dot::Input::IsKeyPressed(D_KEY_DOWN))
		{
			camera.GetPosition() -= glm::vec3(m_CameraMoveSpeed * ts, m_CameraMoveSpeed * ts, m_CameraMoveSpeed * ts) * camera.GetTarget();
		}
		
		

		Dot::Renderer::Clear(glm::vec4(0.4, 0.5, 0.7, 0.0));
	
		transform.SetScale(glm::vec3(1, 1, 1));
		//transform.GetRot().y += 0.01f;
		//transform.GetRot().x += 0.01;
		//transform.GetRot().z += 0.01;
		//transform.GetPos().z = 100;
		camera.UpdateViewMatrix();
	
		//MousePicker.CalculateMouseRay(camera);
		Dot::Renderer::BeginScene(camera);
		{	
			TreeTexture.Bind(1);
			Dot::Renderer::SubmitInstances(InstanceShader, InstanceModel);

			SkyBox->GetTexture().Bind(0);
			Dot::Renderer::SubmitArrays(SkyBoxShader, SkyBox->GetVao());


			GunTexture.Bind(0);
			Dot::Renderer::SubmitElements(WorldShader, Mesh);
			
			m_Anim_Shader->Bind();
		
			dt += 0.005;
			Anim_Transform.InitTranslationTransform(0, 0, dt*10);
			m_Anim_Test->AnimateBones(dt);
	
			m_Anim_Shader->UploadUniformMat4("gBones", (float*)&m_Anim_Test->GetBoneTransformations()[0], m_Anim_Test->GetBoneTransformations().size());
			m_Anim_Shader->UploadUniformMat4("ModelMatrix", (float*)&Anim_Transform);
			CowBoyTex.Bind(0);
			m_Anim_Test->Render();
			TerrTexture.Bind(0);
		}
		Dot::Renderer::EndScene(WorldShader);
		
	}

	void OnEvent(Dot::Event & event) override
	{
		if (event.GetEventType() == Dot::EventType::MouseButtonPressed)
		{
			Dot::MouseButtonPressEvent& e = (Dot::MouseButtonPressEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{
				transform.GetPos().z += 0.1;
				
			}
			if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
			
			}
		}
		else if (event.GetEventType() == Dot::EventType::KeyPressed)
		{
			Dot::KeyPressedEvent& e = (Dot::KeyPressedEvent&)event;
			if (e.GetKey() == D_KEY_LEFT) 
			{
				

			}
			else if (e.GetKey() == D_KEY_RIGHT)
			{ 
				
				
			}
			else if (e.GetKey() == D_KEY_UP)
			{
				
			}
			else if (e.GetKey() == D_KEY_DOWN)
			{
			
			}
		}
		else if (event.GetEventType() == Dot::EventType::MouseScroll)
		{
			Dot::MouseScrollEvent& e = (Dot::MouseScrollEvent&)event;
			if (e.GetValue() > 0)
			{
				
			}
			else if (e.GetValue() < 0)
			{
				
			}
		}

	}

	void OnDetach() override
	{
		
	}

private:
	Dot::Camera camera;
	Dot::Transform transform;

	Dot::Texture TreeTexture;
	Dot::Texture TerrTexture;
	Dot::Texture GunTexture;
	Dot::Texture CowBoyTex;


	Dot::Ref<Dot::InstancedMesh> InstanceModel;
	Dot::Ref<Dot::Shader> InstanceShader;
	std::vector<glm::mat4>InstancePositions;
	


	Dot::Ref<Dot::Shader> SkyBoxShader;
	Dot::Ref<Dot::Skybox> SkyBox;

	Dot::MousePicker MousePicker;
	Dot::Ref<Dot::Mesh> Mesh;
	Dot::Ref<Dot::Shader> WorldShader;

	float m_CameraMoveSpeed = 20.0f;
	float m_CameraRotationSpeed = 2.0f;

	
	Dot::Ref<Dot::Shader> m_Anim_Shader;
	Dot::Ref<Dot::AnimatedMesh> m_Anim_Test  ;
	Dot::Matrix4f Anim_Transform;

	float dt = 0.0f;
};


class SandBox : public Dot::Application
{
public:
	SandBox()
	{
		
		PushLayer(new TestLayer());	
	
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


