#include "stdafx.h"
#include "RenderSystem.h"
#include "Dot/Core/AssetManager.h"
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/Renderer/RenderCommand.h"

namespace Dot {

	RenderSystem::RenderSystem()
	{
		MaterialStack::Init();
	}
	void RenderSystem::BeginScene(const Camera& camera, const Ref<Light>& light)
	{
		m_SceneData.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		m_SceneData.ViewMatrix = camera.GetViewMatrix();
		m_SceneData.ProjectionMatrix = camera.GetProjectionMatrix();
		m_SceneData.ViewPos = camera.GetPosition();

		m_LightData.LightPosition = glm::vec4(light->position, 0);
		m_LightData.LightColor = glm::vec4(light->color, 0);
		m_LightData.LightStrength = light->strength;
	}

	void RenderSystem::Render()
	{	
		for (auto& it : m_Batch.material)
		{
			auto material = MaterialStack::Get()->GetMaterial(it.first);
			setRenderStates(material->GetRenderFlag());

			auto shader = material->GetShader();
			shader->Bind();
			for (int i = 0; i < it.second.size(); ++i)
			{
				auto transform = &ECSManager::Get()->GetComponent<Transform>(it.second[i]);
				shader->UploadUniformMat4("u_ModelMatrix", transform->model);
				auto& component = ECSManager::Get()->GetComponent<RenderComponent>(it.second[i]);
				material->Update();
				component.renderable->Render(shader, component.drawMod);
			}
		}
		for (int i = 0; i < m_FBOs.size(); ++i)
		{
			m_FBOs[i]->Bind();
			RenderCommand::Clear();
			for (auto& it : m_Batch.material)
			{
				auto material = MaterialStack::Get()->GetMaterial(it.first);
				setRenderStates(material->GetRenderFlag());

				auto shader = material->GetShader();
				shader->Bind();
				for (int i = 0; i < it.second.size(); ++i)
				{
					auto transform = &ECSManager::Get()->GetComponent<Transform>(it.second[i]);
					shader->UploadUniformMat4("u_ModelMatrix", transform->model);
					auto& component = ECSManager::Get()->GetComponent<RenderComponent>(it.second[i]);
					material->Update();
					component.renderable->Render(shader, component.drawMod);
				}
			}
		}
		
	}
	void RenderSystem::AddFBO(Ref<Framebuffer> fbo)
	{
		m_FBOs.emplace(m_FBOs.begin() + m_FboIndex, fbo);
		m_FboIndex++;
	}
	void RenderSystem::RemoveFBO(Ref<Framebuffer> fbo)
	{
		auto it = std::find(m_FBOs.begin(), m_FBOs.begin() + m_FboIndex, fbo);
		if (it != m_FBOs.begin() + m_FboIndex)
		{
			m_FBOs.erase(it);
			m_FboIndex--;
		}
	}
	void RenderSystem::Add(Entity entity)
	{
		auto& component = ECSManager::Get()->GetComponent<RenderComponent>(entity);

		LOG_INFO("Entity with ID %d and MaterialID %d pushed to render stack", entity, component.materialID);
		m_Batch.material[component.materialID].push_back(entity);
	}
	void RenderSystem::Remove(Entity entity)
	{
		auto& component = ECSManager::Get()->GetComponent<RenderComponent>(entity);
		if (m_Batch.material.find(component.materialID) != m_Batch.material.end())
		{
			LOG_INFO("Entity with ID %d removed from render stack", entity);
			removeFromContainer(m_Batch.material[component.materialID], entity);
			if (m_Batch.material[component.materialID].empty())
				m_Batch.material.erase(component.materialID);
		}
	}

	void RenderSystem::EndScene(const Ref<Shader>& shader)
	{
		shader->UpdateUniformBufferObject("o_CameraData", &m_SceneData, (sizeof(glm::mat4) * 3) + sizeof(glm::vec3));
		shader->UpdateUniformBufferObject("o_Light", &m_LightData, 2 * sizeof(glm::vec4) + sizeof(float));
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

	void RenderSystem::removeFromContainer(std::vector<Entity>& container, Entity entity)
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