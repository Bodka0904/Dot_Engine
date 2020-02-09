#include "stdafx.h"
#include "LevelEditor.h"
#include "Dot/Renderer/RenderSystem.h"
#include "Dot/Core/Input.h"
#include "Dot/Core/AssetManager.h"

namespace Dot {
	LevelEditor::LevelEditor()
	{
		m_Default = std::make_shared<DefaultUI>();
		m_TerrainEditor = std::make_shared<TerrainEditorUI>();

		m_DefaultID = GuiApplication::Get()->AddBlock(m_Default);
		m_Default->m_TerrainEditorID = GuiApplication::Get()->AddBlock(m_TerrainEditor);
		GuiApplication::Get()->SwitchBlock(m_DefaultID);

	}
	LevelEditor::~LevelEditor()
	{
	}
	void LevelEditor::OnUpdate(float ts)
	{
		m_Default->m_CamController->OnUpdate(ts * 250.0f);
	}
	void LevelEditor::OnRender()
	{
		GuiApplication::Get()->GetCurrent()->OnRender();
	}
	
	void LevelEditor::DefaultUI::OnAttach()
	{
		m_StaticShader = AssetManager::Get()->GetShader("StaticShader");
		m_CamController = std::make_shared<Dot::CameraController>(glm::perspectiveFov(glm::radians(45.0f), float(Dot::Input::GetWindowSize().first) * 0.72f, float(Dot::Input::GetWindowSize().second) * 0.72f, 0.1f, 10000.0f));
		m_Light = std::make_shared<Dot::Light>();
		m_Light->position = glm::vec3(0.0f, 30.0f, 0.0f);
		m_Light->color = glm::vec3(0.7f, 0.7f, 0.7f);
		m_Light->strength = 1.0f;


		m_RenderSystem = ECSManager::Get()->GetSystem<RenderSystem>();

		Dot::Layout layout{
		   {glm::vec2(0.0f,0.0f),glm::vec2(0.2f,1.0f),{
			   {Dot::ElementType::PANEL,0.3f,"Panel"},
			   {Dot::ElementType::PANEL,0.3f,"Panel1"},
			   {Dot::ElementType::PANEL,0.4f,"Panel2"}
		   }},
		   {glm::vec2(0.2f,0.0f),glm::vec2(0.6f,1.0f),{
			   {Dot::ElementType::PANEL,0.08f,"Panel5"},
			   {Dot::ElementType::WINDOW,0.72f,"Scene"},
			   {Dot::ElementType::CONSOLE,0.2f,"Console"},
		   }},
		   {glm::vec2(0.8f,0.0f),glm::vec2(0.2f,1.0f),{
			   {Dot::ElementType::PANEL,0.5f,"Panel3"},
			   {Dot::ElementType::PANEL,0.7f,"Panel4"},
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
		GetPanel("Panel")->AddWidget("Dropdown", Dot::Dropdown::Create("drop", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1)));

		GetPanel("Panel5")->AddWidget("Dropdown1", Dot::Dropdown::Create("drop", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1)));
		//GetPanel("Panel1")->AddWidget("TextArea3", Dot::TextArea::Create("textarea3", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
		//GetPanel("Panel1")->AddWidget("TextArea4", Dot::TextArea::Create("textarea3", glm::vec2(0), glm::vec2(200, 20), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0)));
		//GetPanel("Panel1")->AddWidget("ArrButton", Dot::ArrowButton::Create("arrbutton", glm::vec2(0), glm::vec2(50, 50), glm::vec3(1, 1, 1)));
		//GetPanel("Panel1")->AddWidget("ArrButton2", Dot::ArrowButton::Create("arrbutton", glm::vec2(0), glm::vec2(50, 50), glm::vec3(1, 1, 1)));

		Dot::Logger::Get()->ConnectConsole(GetConsole("Console"));
		GetPanel("Panel")->GetWidget<Dot::Dropdown>("Dropdown").AddBox("Test1");
		GetPanel("Panel")->GetWidget<Dot::Dropdown>("Dropdown").AddBox("Test2");
		GetPanel("Panel")->GetWidget<Dot::Dropdown>("Dropdown").AddBox("Test3");
		GetPanel("Panel")->GetWidget<Dot::Dropdown>("Dropdown").AddBox("Test4");


		GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").AddBox("Test1");
		GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").AddBox("Test2");
		GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").AddBox("Test3");
		GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").AddBox("Test4");
		GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").AddBox("Test5");
		GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").AddBox("Test6");
		GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").AddBox("Test7");
		GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").AddBox("Test8");
	}
	void LevelEditor::DefaultUI::OnUpdate()
	{
		if (GetPanel("Panel5")->GetWidget<Dot::Dropdown>("Dropdown1").Clicked(1))
		{
			GuiApplication::Get()->SwitchBlock(m_TerrainEditorID);
		}	

		
	}


	void LevelEditor::DefaultUI::OnEvent(Event& event)
	{
	}


	void LevelEditor::DefaultUI::OnRender()
	{
		for (auto win : m_Window)
		{
			win.second->ActivateRenderTarget();
			Renderer::Clear(glm::vec4(1, 1, 1, 0));
			m_RenderSystem->BeginScene(m_CamController->GetCamera(), m_Light);
			{
				m_RenderSystem->Render();
			}
			m_RenderSystem->EndScene(m_StaticShader);
			//SceneManager::Get()->GetCurretScene()->OnRender();
		}
		RenderCommand::SetDefaultRenderTarget();
	}
}