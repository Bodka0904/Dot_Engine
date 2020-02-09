#include "stdafx.h"
#include "ParticleSystem.h"
#include "ParticleComponent.h"
#include "Dot/Core/AssetManager.h"
#include "Dot/Utils/Random/RandomGenerator.h"

namespace Dot {
	ParticleSystem::ParticleSystem()
	{
	}
	void ParticleSystem::Update(const Camera& camera, float ts)
	{
		for (auto& it : m_Effect)
		{
			it.first->Bind();
			it.first->UploadUniformFloat("u_Time", ts);
			for (int i = 0; i < it.second.size(); ++i)
			{
				auto& emitter = ECSManager::Get()->GetComponent<ParticleEmitter>(it.second[i].entity);
				it.first->UploadUniformFloat("u_Speed", emitter.speed);
				auto transform = &ECSManager::Get()->GetComponent<Transform>(it.second[i].entity);
				transform->FaceCamera(camera);

				auto& buffData = it.second[i].BufferData;
				buffData.second->BindRange(0, emitter.count * sizeof(ParticleData), buffData.first);

				auto& buffVertex = it.second[i].BufferVertex;
				buffVertex.second->BindRange(0, emitter.count * sizeof(ParticleVertexData), buffVertex.first);

				it.first->Compute(32, 32, 1);
			}
		}
	}
	void ParticleSystem::Add(Entity entity)
	{
		LOG_INFO("Entity with ID %d added to particle system", entity);
		ParticleEffect effect;
		auto compute = &ECSManager::Get()->GetComponent<ComputeComponent>(entity);
		auto emitter = &ECSManager::Get()->GetComponent<ParticleEmitter>(entity);
		auto render = &ECSManager::Get()->GetComponent<RenderComponent>(entity);
		auto transform = &ECSManager::Get()->GetComponent<Transform>(entity);

		effect.entity = entity;
		std::vector<ParticleVertexData> vertexData;
		std::vector<ParticleData> velocityData;
		vertexData.resize(emitter->maxCount);
		velocityData.resize(emitter->maxCount);

		if (emitter->maxCount > MAX_PARTICLES)
		{
			LOG_ERR("Max number of particles per efect is %d", MAX_PARTICLES);
			emitter->maxCount = MAX_PARTICLES;
		}
		for (int i = 0; i < emitter->maxCount; ++i)
		{
			vertexData[i].position = glm::vec4(transform->pos, 1);
			velocityData[i].velocity.x = (float)RandomGenerator::Random(-2, 2);
			velocityData[i].velocity.y = (float)RandomGenerator::Random(4, 7);
			velocityData[i].velocity.z = (float)RandomGenerator::Random(-2, 2);
			velocityData[i].velocity.w = 0;

			velocityData[i].lifeSpan = (float)RandomGenerator::Random(emitter->minLifeSpan, emitter->maxLifeSpan);
		}

		BufferLayout buflayout = {
			{0, ShaderDataType::Float4, "a_ParticlePosition", 1},
		};
		effect.BufferVertex.first = 0;
		effect.BufferVertex.second = ShaderStorageBuffer::Create((float*)vertexData.data(), (unsigned int)vertexData.size() * sizeof(ParticleVertexData), D_DYNAMIC_DRAW);
		effect.BufferVertex.second->SetLayout(buflayout);
		render->renderable->GetVAO()->AddSSBO(effect.BufferVertex.second);


		effect.BufferData.first = 1;
		effect.BufferData.second = ShaderStorageBuffer::Create((float*)velocityData.data(), (unsigned int)velocityData.size() * sizeof(ParticleData), D_DYNAMIC_DRAW);

		m_Effect[compute->shader].push_back(effect);
	}
	void ParticleSystem::Remove(Entity entity)
	{
		auto compute = ECSManager::Get()->GetComponent<ComputeComponent>(entity);
		if (m_Effect.find(compute.shader) != m_Effect.end())
		{
			std::sort(m_Effect[compute.shader].begin(), m_Effect[compute.shader].end(), m_Cmp);
			int position = binarySearch(0, (int)m_Effect.size() - 1, m_Effect[compute.shader], entity);
			if (position != -1 && !m_Effect[compute.shader].empty())
			{
				LOG_INFO("Entity with ID %d removed", entity);
				auto last = m_Effect[compute.shader][m_Effect[compute.shader].size() - 1];
				m_Effect[compute.shader][position].BufferData = last.BufferData;
				m_Effect[compute.shader][position].BufferVertex = last.BufferVertex;
				m_Effect[compute.shader][position].entity = last.entity;
				m_Effect[compute.shader].erase(m_Effect[compute.shader].end() - 1);
			}
			if (m_Effect[compute.shader].empty())
				m_Effect[compute.shader].clear();
		}

	}

	void ParticleSystem::EntityUpdated(Entity entity)
	{
		auto compute = ECSManager::Get()->GetComponent<ComputeComponent>(entity);
		auto emitter = &ECSManager::Get()->GetComponent<ParticleEmitter>(entity);
		auto transform = &ECSManager::Get()->GetComponent<Transform>(entity);

		if (m_Effect.find(compute.shader) != m_Effect.end())
		{
			std::sort(m_Effect[compute.shader].begin(), m_Effect[compute.shader].end(), m_Cmp);
			int position = binarySearch(0, (int)m_Effect.size() - 1, m_Effect[compute.shader], entity);
			if (position != -1 && !m_Effect[compute.shader].empty())
			{
				std::vector<ParticleVertexData> vertexData;
				std::vector<ParticleData> velocityData;
				vertexData.resize(emitter->maxCount);
				velocityData.resize(emitter->maxCount);

				if (emitter->maxCount > MAX_PARTICLES)
				{
					LOG_ERR("Max number of particles per efect is %d", MAX_PARTICLES);
					emitter->maxCount = MAX_PARTICLES;
				}
				for (int i = 0; i < emitter->maxCount; ++i)
				{
					vertexData[i].position = glm::vec4(transform->pos, 1);
					velocityData[i].velocity.x = (float)RandomGenerator::Random(-2, 2);
					velocityData[i].velocity.y = (float)RandomGenerator::Random(4, 7);
					velocityData[i].velocity.z = (float)RandomGenerator::Random(-2, 2);
					velocityData[i].velocity.w = 0;
					velocityData[i].lifeSpan = (float)RandomGenerator::Random(emitter->minLifeSpan, emitter->maxLifeSpan);
				}
				m_Effect[compute.shader][position].BufferVertex.second->Resize((void*)& vertexData[0], emitter->maxCount * sizeof(ParticleVertexData));
				m_Effect[compute.shader][position].BufferData.second->Resize((void*)& velocityData[0], emitter->maxCount * sizeof(ParticleData));
			}
		}
	}

	int ParticleSystem::binarySearch(int start, int end, const std::vector<ParticleEffect>& container, Entity entity)
	{
		if (end >= start)
		{
			int mid = start + (end - start) / 2;
			if (container[mid].entity == entity)
				return mid;

			if (container[mid].entity > entity)
				return binarySearch(start, mid - 1, container, entity);


			return binarySearch(mid + 1, end, container, entity);
		}
		return -1;
	}



}