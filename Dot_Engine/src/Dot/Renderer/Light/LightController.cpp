#include "stdafx.h"
#include "LightController.h"

#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/Input.h"
#include "Dot/Core/AssetManager.h"
#include "Dot/Utils/Math/Transform.h"
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/Checkbox.h"

namespace Dot {
	LightController::LightController(const glm::vec3& position, const glm::vec3& color)
	{
		m_Light = std::make_shared<Light>(position, color);
		m_Mesh = AssetManager::Get()->GetMesh("Grass");
		m_Model = CalculateModel(m_Light->GetPosition());

		Wrapper::Create("Light", glm::vec2(500, 500), glm::vec2(300, 300));
		Gui::Get()->EnableWrapper("Light");
		{
			Slider::Create("Strength", glm::vec2(50, 50), glm::vec2(200, 20), &m_Light->GetStrength());
			Checkbox::Create("Control", glm::vec2(50, 50), glm::vec2(50, 50));

		}Gui::Get()->DisableWrapper();
		
	}
	void LightController::Update(float dt)
	{
		if (Checkbox::GetWrapped("Light", "Control").GetClicked())
		{
			if (Input::IsKeyPressed(D_KEY_W))
			{
				m_Light->GetPosition().z += m_Speed * dt;
				m_Model = CalculateModel(m_Light->GetPosition());
			}
			else if (Input::IsKeyPressed(D_KEY_S))
			{
				m_Light->GetPosition().z -= m_Speed * dt;
				m_Model = CalculateModel(m_Light->GetPosition());
			}

			if (Input::IsKeyPressed(D_KEY_A))
			{
				m_Light->GetPosition().x += m_Speed * dt;
				m_Model = CalculateModel(m_Light->GetPosition());
			}
			else if (Input::IsKeyPressed(D_KEY_D))
			{
				m_Light->GetPosition().x -= m_Speed * dt;
				m_Model = CalculateModel(m_Light->GetPosition());
			}

			if (Input::IsKeyPressed(D_KEY_KP_2))
			{
				m_Light->GetPosition().y += m_Speed * dt;
				m_Model = CalculateModel(m_Light->GetPosition());
			}
			else if (Input::IsKeyPressed(D_KEY_KP_0))
			{
				m_Light->GetPosition().y -= m_Speed * dt;
				m_Model = CalculateModel(m_Light->GetPosition());
			}
		}
	}
	const glm::mat4& LightController::GetModel()
	{
		return m_Model;
	}
}