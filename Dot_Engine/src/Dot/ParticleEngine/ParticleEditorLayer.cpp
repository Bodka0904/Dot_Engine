#include "stdafx.h"
#include "ParticleEditorLayer.h"


#include "Dot/Gui/GuiSystem/Button.h"
#include "Dot/Gui/GuiSystem/Arrowbutton.h"
#include "Dot/Gui/GuiSystem/Checkbox.h"
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/Textarea.h"

#include "Dot/Core/Input.h"
#include "Dot/Renderer/RenderSystem.h"
#include "Dot/Core/AssetManager.h"


namespace Dot {
	void ParticleEditorLayer::OnAttach()
	{
		m_Entity.second = ECSManager::Get()->CreateEntity();
		Dot::ECSManager::Get()->AddComponent(m_Entity.second, Dot::ParticleComponent{
			"ParticleComputeShader",
			500,
			200,
			1.0,
			1.0,
			2,
			3,
			ParticleAttribute::SMALLER
			});

		m_ParComponent = &ECSManager::Get()->GetComponent<ParticleComponent>(m_Entity.second);
		m_Effect = std::make_shared<Dot::ParticleEffect>(*m_ParComponent);
		m_Material = std::make_shared<Dot::Material>(AssetManager::Get()->GetShader("ParticleShader"));
		m_Material->Set(RenderMode::ADDITIVE | RenderMode::TRANSLUCENT);
		
		Dot::ECSManager::Get()->AddComponent(m_Entity.second, m_Effect);
		Dot::ECSManager::Get()->AddComponent(m_Entity.second, Dot::RenderComponent{
			m_Effect,
			m_Material
			});

		Dot::ECSManager::Get()->AddComponent(m_Entity.second, Dot::Transform{
			glm::vec3(0, 0, 0),
			glm::vec3(0, 0, 0), 
			glm::vec3(1, 1, 1) 
			});
		m_TransformComponent = &ECSManager::Get()->GetComponent<Transform>(m_Entity.second);	
	}
	void ParticleEditorLayer::OnUpdate(Timestep ts)
	{		
		
	}
	void ParticleEditorLayer::OnDetach()
	{
	}
	void ParticleEditorLayer::OnEvent(Event& event)
	{
				
	}
	void Test()
	{
		std::cout << "Test" << std::endl;
	}
	void Dot::ParticleEditorLayer::OnGuiAttach()
	{
		
		Console::Create("Console", glm::vec2(300, 300), glm::vec2(300, 300));
		Gui::Get()->GetConsole("Console").PushText("Hola hop krava opica retard dement haha");
		Gui::Get()->GetConsole("Console").RegisterCommand("Test",Test);

		glm::vec2 winSize(Input::GetWindowSize().first, Input::GetWindowSize().second);
		glm::vec2 wrapSizeEffect(300, 450);
		Dot::Wrapper::Create("Particle Effect", winSize - wrapSizeEffect, wrapSizeEffect, 8);
		Gui::Get()->EnableWrapper("Particle Effect");
		{
			TextArea::Create("Name", glm::vec2(50, 50), glm::vec2(200, 20), TextArea::TYPE::TEXT);
			TextArea::Create("Texture", glm::vec2(50, 50), glm::vec2(200, 20), TextArea::TYPE::TEXT);
			TextArea::Create("Max Particles", glm::vec2(50, 50), glm::vec2(200, 20), TextArea::TYPE::INT);
			TextArea::Create("Num Particles", glm::vec2(50, 50), glm::vec2(200, 20), TextArea::TYPE::INT);
			Slider::Create("Gravity", glm::vec2(50, 50), glm::vec2(200, 20), &m_ParComponent->gravity, 10);
			Slider::Create("Speed", glm::vec2(50, 50), glm::vec2(200, 20), &m_ParComponent->speed, 10);
			Button::Create("Save", glm::vec2(300, 300), glm::vec2(50, 50));

		}Gui::Get()->DisableWrapper();

		TextArea& m_NumPar = TextArea::GetWrapped("Particle Effect", "Num Particles");
		TextArea& m_MaxPar = TextArea::GetWrapped("Particle Effect", "Max Particles");
		m_NumPar.SetText(std::to_string(m_ParComponent->numParticles));
		m_MaxPar.SetText(std::to_string(m_Effect->GetMaxParticles()));

		glm::vec2 wrapSizeTransform(300, 300);
		glm::vec2 wrapSizePos = glm::vec2(winSize - wrapSizeTransform) - glm::vec2(0, wrapSizeEffect.y);
		Dot::Wrapper::Create("Transform", wrapSizePos, wrapSizeTransform, 3);
		Gui::Get()->EnableWrapper("Transform");
		{
			TextArea::Create("Pos x", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
			TextArea::Create("Pos y", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
			TextArea::Create("Pos z", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);

			TextArea::Create("Rot x", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
			TextArea::Create("Rot y", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
			TextArea::Create("Rot z", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);

			TextArea::Create("Scale x", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
			TextArea::Create("Scale y", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);
			TextArea::Create("Scale z", glm::vec2(0, 0), glm::vec2(70, 20), TextArea::TYPE::FLOAT);

		}Gui::Get()->DisableWrapper();

		glm::vec2 wrapSizeTime(150, 300);
		glm::vec2 wrapTimePos = glm::vec2(winSize.x - wrapSizeTime.x - wrapSizeTransform.x, winSize.y - wrapSizeTime.y);
		Dot::Wrapper::Create("Values Time", wrapTimePos, wrapSizeTime, 3);
		Gui::Get()->EnableWrapper("Values Time");
		{
			Dot::Button::Create("None Size", glm::vec2(300, 300), glm::vec2(50, 50));
			Dot::Button::Create("Smaller Size", glm::vec2(300, 300), glm::vec2(50, 50));
			Dot::Button::Create("Bigger Size", glm::vec2(300, 300), glm::vec2(50, 50));
		}
		Gui::Get()->DisableWrapper();
	}
	void Dot::ParticleEditorLayer::OnGuiUpdate()
	{
		if (Button::GetWrapped("Particle Effect", "Save").GetClicked())
		{
			m_Effect->UpdateParticleData(*m_ParComponent);
			std::string texture = TextArea::GetWrapped("Particle Effect", "Texture").GetValue<std::string>();
			auto& component = ECSManager::Get()->GetComponent<Dot::RenderComponent>(m_Entity.second);
			component.material->Set(AssetManager::Get()->GetTexture(texture));
		}
		else if (Button::GetWrapped("Values Time", "None Size").GetClicked())
		{
			m_ParComponent->sizeOverLifeTime = ParticleAttribute::NONE;
		}
		else if (Button::GetWrapped("Values Time", "Smaller Size").GetClicked())
		{
			m_ParComponent->sizeOverLifeTime = ParticleAttribute::SMALLER;
		}
		else if (Button::GetWrapped("Values Time", "Bigger Size").GetClicked())
		{
			m_ParComponent->sizeOverLifeTime = ParticleAttribute::BIGGER;
		}
	}
	void Dot::ParticleEditorLayer::OnGuiDetach()
	{
		ECSManager::Get()->DestroyEntity(m_Entity.second);
		Gui::Get()->RemoveWrapper("Particle Effect");
		Gui::Get()->RemoveWrapper("Transform");
		Gui::Get()->RemoveWrapper("Values Time");	
	}
	void Dot::ParticleEditorLayer::OnGuiEvent(Event& event)
	{
		if (event.GetEventType() == EventType::KeyPressed)
		{
			
			TextArea& m_Name = TextArea::GetWrapped("Particle Effect", "Name");
			TextArea& m_Texture = TextArea::GetWrapped("Particle Effect", "Texture");
			TextArea& m_NumPar = TextArea::GetWrapped("Particle Effect", "Num Particles");
			TextArea& m_MaxPar = TextArea::GetWrapped("Particle Effect", "Max Particles");

			TextArea& m_PosX = TextArea::GetWrapped("Transform", "Pos x");
			TextArea& m_PosY = TextArea::GetWrapped("Transform", "Pos y");
			TextArea& m_PosZ = TextArea::GetWrapped("Transform", "Pos z");

			TextArea& m_RotX = TextArea::GetWrapped("Transform", "Rot x");
			TextArea& m_RotY = TextArea::GetWrapped("Transform", "Rot y");
			TextArea& m_RotZ = TextArea::GetWrapped("Transform", "Rot z");

			TextArea& m_ScaleX = TextArea::GetWrapped("Transform", "Scale x");
			TextArea& m_ScaleY = TextArea::GetWrapped("Transform", "Scale y");
			TextArea& m_ScaleZ = TextArea::GetWrapped("Transform", "Scale z");

			KeyPressedEvent& e = (KeyPressedEvent&)event;
	
			if (Gui::Get()->GetConsole("Console").GetClicked())
			{
				Gui::Get()->GetConsole("Console").TakeInput(e);
			}
			if (m_Name.GetClicked())
			{
				m_Name.TakeInput(e);
				m_Entity.first = m_Name.GetValue<std::string>();
				e.IsHandled() = true;
			}
			else if (m_Texture.GetClicked())
			{
				m_Texture.TakeInput(e);
				e.IsHandled() = true;
			}
			else if (m_MaxPar.GetClicked())
			{
				m_MaxPar.TakeInput(e);
				int num = m_MaxPar.GetValue<int>();
				m_ParComponent->maxParticles = num;
				e.IsHandled() = true;
			}
			else if (m_NumPar.GetClicked())
			{
				m_NumPar.TakeInput(e);
				int num = m_NumPar.GetValue<int>();
				m_ParComponent->numParticles = num;
				m_Effect->SetRenderCount(num);
				e.IsHandled() = true;
			}
			else if (m_PosX.GetClicked())
			{
				m_PosX.TakeInput(e);
				m_TransformComponent->pos.x = m_PosX.GetValue<float>();
				e.IsHandled();
			}
			else if (m_PosY.GetClicked())
			{
				m_PosY.TakeInput(e);
				m_TransformComponent->pos.y = m_PosY.GetValue<float>();
				e.IsHandled();
			}
			else if (m_PosZ.GetClicked())
			{
				m_PosZ.TakeInput(e);
				m_TransformComponent->pos.z = m_PosZ.GetValue<float>();
				e.IsHandled();
			}
			else if (m_RotX.GetClicked())
			{
				m_RotX.TakeInput(e);
				m_TransformComponent->rot.x = m_RotX.GetValue<float>();
				e.IsHandled();
			}
			else if (m_RotY.GetClicked())
			{
				m_RotY.TakeInput(e);
				m_TransformComponent->rot.y = m_RotY.GetValue<float>();
				e.IsHandled();
			}
			else if (m_RotZ.GetClicked())
			{
				m_RotZ.TakeInput(e);
				m_TransformComponent->rot.z = m_RotZ.GetValue<float>();
				e.IsHandled();
			}
			else if (m_ScaleX.GetClicked())
			{
				m_ScaleX.TakeInput(e);
				m_TransformComponent->scale.x = m_ScaleX.GetValue<float>();
				e.IsHandled();
			}
			else if (m_ScaleY.GetClicked())
			{
				m_ScaleY.TakeInput(e);
				m_TransformComponent->scale.y = m_ScaleY.GetValue<float>();
				e.IsHandled();
			}
			else if (m_ScaleZ.GetClicked())
			{
				m_ScaleZ.TakeInput(e);
				m_TransformComponent->scale.z = m_ScaleZ.GetValue<float>();
				e.IsHandled();
			}
		}

	}
	std::pair<std::string, Entity> Dot::ParticleEditorLayer::GetEntity()
	{
		m_Effect->UpdateParticleData(*m_ParComponent);
		std::string texture = TextArea::GetWrapped("Particle Effect", "Texture").GetValue<std::string>();
		auto& comp = ECSManager::Get()->GetComponent<Dot::RenderComponent>(m_Entity.second);
		comp.material->Set(AssetManager::Get()->GetTexture(texture));

		return m_Entity;
	}
}

