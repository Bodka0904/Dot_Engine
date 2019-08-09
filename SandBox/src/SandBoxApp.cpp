#pragma once
#include <Dot.h>


class TestLayer : public Dot::Layer
{
public:
	TestLayer()
		: 
		Dot::Layer(),
		camera(50.0f,1280.0f/720.0f,1.0f,1500.0f)
	{

	}

	void OnAttach()
	{
		Dot::BufferLayout layout = {
				{0, Dot::ShaderDataType::Float3, "position" },
				{1, Dot::ShaderDataType::Float3, "normal" },
				{2, Dot::ShaderDataType::Float2, "texCoord" },
				
		};
		cube = std::make_shared<Dot::Mesh>("res/models/Dot/bear.obj", layout);
		

		Dot::ShaderLayout s_layout = {
			{0,"position"},
			{1,"normal"},
			{2,"texCoord"},
	
		};
		
		WorldShader = std::make_shared<Dot::Shader>("res/shaders/Dot/TestGeometryShader");
		WorldShader->AddGeometryShader("res/shaders/Dot/GeometryShader.gs");
		WorldShader->SetLayout(s_layout);
		WorldShader->LinkShader();
	
		WorldShader->Bind();
		WorldShader->SetUniforms();
		WorldShader->AddUniformBufferObject("camera_data", 0,3*sizeof(glm::mat4));
		WorldShader->AddUniform("ModelMatrix");
		WorldShader->AddUniform("time");
			
		texture.Create2D("res/textures/Dot/treehill.png");
		

		Dot::ShaderLayout test_layout = {
			{0,"position"},
			{1,"normal"},
			{2,"texCoord"},
			{3,"instanceModel"},
		};

		InstanceShader = std::make_shared<Dot::Shader>("res/shaders/Dot/InstancedShader");
		InstanceShader->SetLayout(test_layout);
		InstanceShader->LinkShader();
	
		InstanceShader->Bind();
		InstanceShader->AddUniform("texi");
		InstanceShader->UploadInt("texi", 1);
		
		Dot::BufferLayout layout_test = {
				{0, Dot::ShaderDataType::Float3, "position" },
				{1, Dot::ShaderDataType::Float3, "normal" },
				{2, Dot::ShaderDataType::Float2, "texCoord" },
			
		};
		
		test_positions.resize(5);		
		transform.GetPos().x += 30;
		transform.SetScale(glm::vec3(10, 10, 10));
		test_positions[0] = transform.GetModel();
		transform.GetPos().x += 30;
		test_positions[1] = transform.GetModel();
		transform.GetPos().x += 30;
		test_positions[2] = transform.GetModel();
		transform.GetPos().x += 30;
		test_positions[3] = transform.GetModel();
		transform.GetPos().x = 0;

		bearTex.Create2D("res/textures/Dot/bear.png");
		test = std::make_shared<Dot::InstancedMesh>("res/models/Dot/treehill.obj", layout_test, test_positions);

		Dot::ShaderLayout skyLayout = {
			{0,"position"}
		};

		SkyBoxShader = std::make_shared<Dot::Shader>("res/shaders/Dot/SkyboxShader");
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
	
		transform.SetScale(glm::vec3(20, 20, 20));
		//transform.GetRot().y += 0.01f;
		//transform.GetRot().x += 0.01;
		//transform.GetRot().z += 0.01;
		//transform.GetPos().z = 100;
		camera.UpdateViewMatrix();
		cube->SetModelMatrix(transform.GetModel());

		Dot::Renderer::BeginScene(camera);
		{
			bearTex.Bind(0);
			Dot::Renderer::SubmitElements(WorldShader,cube);
			
			texture.Bind(1);
			Dot::Renderer::SubmitInstances(InstanceShader, test);

			
			SkyBox->GetTexture().Bind(0);
			Dot::Renderer::SubmitArrays(SkyBoxShader, SkyBox->GetVao());
	

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
	Dot::Texture texture;
	Dot::Texture bearTex;
	std::shared_ptr<Dot::Mesh> cube;
	std::shared_ptr<Dot::InstancedMesh> test;
	std::shared_ptr<Dot::Shader> InstanceShader;
	std::vector<glm::mat4>test_positions;
	std::shared_ptr<Dot::Shader> WorldShader;


	std::shared_ptr<Dot::Shader> SkyBoxShader;
	std::shared_ptr<Dot::Skybox> SkyBox;

	float m_CameraMoveSpeed = 20.0f;
	float m_CameraRotationSpeed = 2.0f;
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


