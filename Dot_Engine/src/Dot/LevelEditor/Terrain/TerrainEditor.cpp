#include "stdafx.h"
#include "TerrainEditor.h"
#include "Dot/Utils/Random/RandomGenerator.h"
#include "Dot/Core/Input.h"
#include "Dot/Utils/Image/ImageLoader.h"

#include "Dot/Core/KeyCodes.h"
#include "Dot/Core/MouseButtonCodes.h"
#include "Dot/Renderer/RenderSystem.h"


#include "Dot/Core/AssetManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>



namespace Dot {
	void TerrainEditorUI::OnAttach()
	{
		m_Entity = ECSManager::Get()->CreateEntity();
		
		ECSManager::Get()->AddComponent(m_Entity, RenderComponent{
			std::make_shared<Terrain>(250,250),
			Dot::MaterialStack::Get()->GetMaterialID("Default"),
			D_TRIANGLES
			});
		ECSManager::Get()->AddComponent(m_Entity, TerrainCollider{});
		auto collider = &ECSManager::Get()->GetComponent<TerrainCollider>(m_Entity);
		collider->UpdateHeights(m_Entity);

		Dot::Layout layout
		{
			{glm::vec2(0.0f,0.0f),glm::vec2(0.2f,1.0f),{
				{Dot::ElementType::PANEL,0.3f,"Panel"},
				{Dot::ElementType::PANEL,0.3f,"Panel1"},
				{Dot::ElementType::PANEL,0.4f,"Panel2"}
			}},
			{glm::vec2(0.2f,0.0f),glm::vec2(0.8f,1.0f),{
				{Dot::ElementType::PANEL,0.08f,"Brushes"},
				{Dot::ElementType::WINDOW,0.92f,"Terrain Editor"},
			}},
		};


		SetLayout(layout);
	}

	void TerrainEditorUI::OnUpdate()
	{
	}
	void TerrainEditorUI::OnEvent(Event& event)
	{
	}
}