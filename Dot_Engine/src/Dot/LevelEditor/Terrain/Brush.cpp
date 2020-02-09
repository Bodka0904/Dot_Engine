#include "stdafx.h"
#include "Brush.h"
#include "Dot/Utils/Random/RandomGenerator.h"
#include "Dot/Core/AssetManager.h"
#include "Dot/Renderer/RenderSystem.h"

namespace Dot {
	Brush::Brush(unsigned int intensity, float radius, float density, Ref<Terrain> terrain)
		:
		m_Intensity(intensity),
		m_Radius(radius),
		m_Density(density)
	{
		m_Terrain = terrain;


		m_Entity = ECSManager::Get()->CreateEntity();
		ECSManager::Get()->AddComponent(m_Entity, Transform{ glm::vec3(0, 0, 0),glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) });
		ECSManager::Get()->AddComponent(m_Entity, Dot::RigidBody{ glm::vec3{0, 0, 0} });
		m_Transform = &ECSManager::Get()->GetComponent<Transform>(m_Entity);
	}
	Brush::~Brush()
	{
		ECSManager::Get()->DestroyEntity(m_Entity);
	}
	void Brush::Place(const Ref<BrushInstance>& instance)
	{
		std::vector<glm::mat4> transforms;
		transforms.resize(m_Intensity);

		Transform tmpTransform;
		tmpTransform.pos = m_Transform->pos;
		tmpTransform.scale = glm::vec3(1, 1, 1);
		tmpTransform.rot = glm::vec3(0, 0, 0);

		float step = m_Radius / m_Intensity;
		for (size_t i = 0; i < m_Intensity; ++i)
		{
			glm::vec2 nextPos;
			nextPos.x = step * (i + 1) * cos(i) + m_Transform->pos.x + (float)RandomGenerator::Random(0.0, (double)m_Density);
			nextPos.y = step * (i + 1) * sin(i) + m_Transform->pos.z + (float)RandomGenerator::Random(0.0, (double)m_Density);

			//tmpTransform.pos = glm::vec3(nextPos.x, m_Terrain->GetHeight(glm::vec3(nextPos.x, 0, nextPos.y)), nextPos.y);
			tmpTransform.rot.y = (float)RandomGenerator::Random(0, 90);
			tmpTransform.UpdateModel();
			transforms[i] = tmpTransform.model;
		}
		instance->Update(transforms);
	}
	void Dot::Brush::SetPosition(const glm::vec3& position)
	{
		m_Transform->pos = position;
	}
	void Brush::SetIntensity(unsigned int intensity)
	{
		m_Intensity = intensity;
	}
	void Brush::SetRadius(float radius)
	{
		m_Radius = radius;
	}
	void Brush::SetDensity(float density)
	{
		m_Density = density;
	}
}