#include "stdafx.h"
#include "RenderSystem.h"
#include "Dot/Core/AssetManager.h"
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/Renderer/RenderCommand.h"

namespace Dot {

	RenderSystem::RenderSystem()
	{
		
	}
	void RenderSystem::BeginScene(const Camera& camera, const Ref<Light>& light)
	{
		m_SceneData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		m_SceneData.ViewMatrix = camera.GetViewMatrix();
		m_SceneData.ProjectionMatrix = camera.GetProjectionMatrix();
		m_SceneData.ViewPos = camera.GetPosition();

		m_LightData.LightPosition = glm::vec4(light->GetPosition(),0);
		m_LightData.LightColor = glm::vec4(light->GetColor(),0);
		m_LightData.LightStrength = light->GetStrength();
	}

	void RenderSystem::Render()
	{
		for (auto& it : m_BatchOpaque)
		{
			setRenderStates(it.first.second);
			auto comp = ECSManager::Get()->GetComponent<RenderComponent>(it.second.entities[0]);
			auto shader = comp.material->GetShader();
			shader->Bind();
			for (int i = 0; i < it.second.entities.size(); ++i)
			{
				auto& transform = ECSManager::Get()->GetComponent<Transform>(it.second.entities[i]);
				shader->UploadUniformMat4("u_ModelMatrix",transform.model);
				auto& component = ECSManager::Get()->GetComponent<RenderComponent>(it.second.entities[i]);
				comp.material->Update();
				component.mesh->Render(shader);
			}
		}
		for (auto& it : m_BatchTransparent)
		{
			setRenderStates(it.first.second);
			auto comp = ECSManager::Get()->GetComponent<RenderComponent>(it.second.entities[0]);
			auto shader = comp.material->GetShader();
			shader->Bind();
			for (int i = 0; i < it.second.entities.size(); ++i)
			{
				auto& transform = ECSManager::Get()->GetComponent<Transform>(it.second.entities[i]);
				shader->UploadUniformMat4("u_ModelMatrix", transform.model);
				auto& component = ECSManager::Get()->GetComponent<RenderComponent>(it.second.entities[i]);
				comp.material->Update();
				component.mesh->Render(shader);
			}
		}
		
	}
	void RenderSystem::Add(Entity entity)
	{
		auto& component = ECSManager::Get()->GetComponent<RenderComponent>(entity);
		std::pair<uint32_t, int32_t> key(component.material->GetID(), component.material->GetRenderFlag());
		
		if (component.material->GetRenderFlag() | OPAQUE)
			m_BatchOpaque[key].entities.push_back(entity);
		else
			m_BatchTransparent[key].entities.push_back(entity);
	
		

	}
	void RenderSystem::Remove(Entity entity)
	{
		auto& component = ECSManager::Get()->GetComponent<RenderComponent>(entity);
		std::pair<uint32_t, int32_t> key(component.material->GetID(), component.material->GetRenderFlag());
		
		if (component.material->GetRenderFlag() | OPAQUE)
		{
			removeFromContainer(m_BatchOpaque[key].entities, entity);
			if (m_BatchOpaque[key].entities.empty())
				m_BatchOpaque.erase(key);
		}
		else
		{
			removeFromContainer(m_BatchTransparent[key].entities, entity);
			if (m_BatchOpaque[key].entities.empty())
				m_BatchOpaque.erase(key);
		}
	}

	void RenderSystem::EndScene(const Ref<Shader>& shader)
	{
		shader->UpdateUniformBufferObject("o_CameraData", &m_SceneData, (sizeof(glm::mat4) * 3) + sizeof(glm::vec3));
		shader->UpdateUniformBufferObject("o_Light", &m_LightData, 2*sizeof(glm::vec4) + sizeof(float));
	}
	
	
	void RenderSystem::setRenderStates(int32_t states)
	{
		if (states & ADDITIVE)
		{
			RenderCommand::SetBlendFunc(D_SRC_ALPHA, D_ONE);
		}
		else
		{
			RenderCommand::SetBlendFunc(D_SRC_ALPHA, D_ONE_MINUS_SRC_ALPHA);
		}
	}

	void RenderSystem::removeFromContainer(std::vector<Entity>& container,Entity entity)
	{
		std::sort(container.begin(), container.end(), m_Cmp);
		// get the range in 2*log2(N), N=vec.size()
		auto bounds = std::equal_range(container.begin(), container.end(), entity);
		// calculate the index of the first to be deleted O(1)
		auto last = container.end() - std::distance(bounds.first, bounds.second);
		// swap the 2 ranges O(equals) , equal = std::distance(bounds.first, bounds.last)
		std::swap_ranges(bounds.first, bounds.second, last);
		// erase the victims O(equals)
		container.erase(last, container.end());
	}

}