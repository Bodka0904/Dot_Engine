#pragma once
#include <Dot.h>


class TestLayer : public Dot::Layer
{
public:
	TestLayer()
		: 
		Dot::Layer(),
		camera(50.0f,1280.0f/720.0f,0.01f,1000.0f)
	{

	}

	void OnAttach()
	{
		Dot::BufferLayout layout = {
				{ Dot::ShaderDataType::Float3, "position" },
				{ Dot::ShaderDataType::Float3, "normal" },
				{ Dot::ShaderDataType::Float2, "texCoord" },
				
		};
		cube.reset(new Dot::Mesh("res/models/Dot/test.obj", layout));
		

		Dot::ShaderLayout s_layout = {
			{0,"position"},
			{1,"normal"},
			{2,"texCoord"},
	
		};
		
		WorldShader.reset(new Dot::WorldShader("res/shaders/Dot/TestGeometryShader"));
		WorldShader->AddGeometryShader("res/shaders/Dot/GeometryShader.gs");
		WorldShader->SetLayout(s_layout);
		WorldShader->LinkShader();
		WorldShader->Clean();
		WorldShader->Bind();
		WorldShader->SetUniforms();
		WorldShader->AddUniformBufferObject("camera_data", 0,sizeof(glm::mat4));
		WorldShader->AddUniform("ModelMatrix");
		WorldShader->AddUniform("time");
		
		texture.Create("res/textures/Dot/grass.jpg");
		texture.Bind(0);

		
	}

	void OnUpdate() override
	{
		//CAMERA CONTROL - DEBUG PURPOSE
		
		if (Dot::Input::IsKeyPressed(D_KEY_W))
		{
			camera.GetTarget().y += 0.005;
		}
		if (Dot::Input::IsKeyPressed(D_KEY_S))
		{
			camera.GetTarget().y -= 0.005;
		}
		if (Dot::Input::IsKeyPressed(D_KEY_A))
		{
			camera.GetRotation().y += 0.01;
			camera.GetTarget().x = sin(camera.GetRotation().y);
			camera.GetTarget().z = cos(camera.GetRotation().y);
		}
		if (Dot::Input::IsKeyPressed(D_KEY_D))
		{
			camera.GetRotation().y -= 0.01;
			camera.GetTarget().x = sin(camera.GetRotation().y);
			camera.GetTarget().z = cos(camera.GetRotation().y);
		}
		if (Dot::Input::IsKeyPressed(D_KEY_UP))
		{
			camera.GetPosition() += glm::vec3(0.1, 0.1, 0.1) * camera.GetTarget();
		}
		if (Dot::Input::IsKeyPressed(D_KEY_DOWN))
		{
			camera.GetPosition() -= glm::vec3(0.1, 0.1, 0.1) * camera.GetTarget();
		}
		
		

		Dot::Renderer::Clear(glm::vec4(0.4, 0.5, 0.7, 0.0));

		texture.Bind(0);
	
		//transform.SetScale(glm::vec3(1, 1, 1));
		//transform.GetRot().y += 0.01f;
		//transform.GetRot().x += 0.01;
		//transform.GetRot().z += 0.01;
		//transform.GetPos().z = 100;
		camera.UpdateViewMatrix();
		Dot::Renderer::BeginScene(camera);
		{
			Dot::Renderer::Submit(WorldShader,transform,cube);		
		}
		Dot::Renderer::EndScene(WorldShader);
	}

	void OnEvent(Event & event) override
	{
		
		if (event.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressEvent& e = (MouseButtonPressEvent&)event;
			if (e.GetButton() == D_MOUSE_BUTTON_LEFT)
			{
				
			}
			if (e.GetButton() == D_MOUSE_BUTTON_RIGHT)
			{
				
			}
		}
		else if (event.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& e = (KeyPressedEvent&)event;
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
		else if (event.GetEventType() == EventType::MouseScroll)
		{
			MouseScrollEvent& e = (MouseScrollEvent&)event;
			if (e.GetValue() > 0)
			{
				camera.GetPosition().y += 0.05;
			}
			else if (e.GetValue() < 0)
			{
				camera.GetPosition().y -= 0.05;
			}
		}

	}

private:
	Dot::Camera camera;
	Dot::Transform transform;
	Dot::Texture texture;
	std::shared_ptr<Dot::Mesh> cube;
	std::shared_ptr<Dot::WorldShader> WorldShader;
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


