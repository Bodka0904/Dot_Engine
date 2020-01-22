#include "stdafx.h"
#include "LightController.h"

#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/Input.h"
#include "Dot/Core/AssetManager.h"
#include "Dot/Utils/Math/Transform.h"


namespace Dot {
	LightController::LightController(const glm::vec3& position, const glm::vec3& color)
	{
		m_Light = std::make_shared<Light>(position, color);
		m_Mesh = AssetManager::Get()->GetCopyStaticMesh("Grass");
		m_Model = CalculateModel(m_Light->GetPosition());

		
	}
	void LightController::Update(float dt)
	{
	
		
	}
	const glm::mat4& LightController::GetModel()
	{
		return m_Model;
	}
}