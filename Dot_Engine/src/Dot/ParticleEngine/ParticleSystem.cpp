#include "stdafx.h"
#include "ParticleSystem.h"

#include "Dot/Core/AssetManager.h"

namespace Dot {
	ParticleSystem::ParticleSystem()
	{
	}
	void ParticleSystem::Update(const Camera& camera, float ts)
	{
		for (auto& it : m_Entities)
		{
			auto shader = AssetManager::Get()->GetShader(it.first);
			shader->Bind();
			shader->UploadUniformFloat("u_Time", ts);
			for (int i = 0; i < it.second.size(); ++i)
			{
				auto& component = ECSManager::Get()->GetComponent<ParticleComponent>(it.second[i]);
				shader->UploadUniformFloat("u_Speed",component.speed);
				auto& transform = ECSManager::Get()->GetComponent<Dot::Transform>(it.second[i]);
				transform.FaceCamera(camera);
				auto& effect = ECSManager::Get()->GetComponent<Dot::Ref<Dot::ParticleEffect>>(it.second[i]);
				effect->SwapBuffers();
				shader->Compute(32, 32, 1);	

			}
		}
	}
	void ParticleSystem::Add(Entity entity)
	{
		auto component = ECSManager::Get()->GetComponent<Dot::ParticleComponent>(entity);
		m_Entities[component.computeShader].push_back(entity);
	}
	void ParticleSystem::Remove(Entity entity)
	{
		auto component = ECSManager::Get()->GetComponent<Dot::ParticleComponent>(entity);
		if (m_Entities.find(component.computeShader) != m_Entities.end())
		{

			std::sort(m_Entities[component.computeShader].begin(), m_Entities[component.computeShader].end(), m_Cmp);
			auto bounds = std::equal_range(m_Entities[component.computeShader].begin(), m_Entities[component.computeShader].end(), entity);
			// calculate the index of the first to be deleted O(1)
			auto last = m_Entities[component.computeShader].end() - std::distance(bounds.first, bounds.second);
			// swap the 2 ranges O(equals) , equal = std::distance(bounds.first, bounds.last)
			std::swap_ranges(bounds.first, bounds.second, last);
			// erase the victims O(equals)
			m_Entities[component.computeShader].erase(last, m_Entities[component.computeShader].end());
			
			if (m_Entities[component.computeShader].empty())
				m_Entities.clear();
		}
		
	}
	
}