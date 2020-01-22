#include "stdafx.h"
#include "ParticleEditor.h"
#include "ParticleComponent.h"


#include "Dot/Core/AssetManager.h"
#include "Dot/Core/Input.h"
#include "Dot/Renderer/RenderSystem.h"

namespace Dot {
	void ParticleEditor::OnCreate(Entity entity)
	{
		m_Entity = entity;

		////////////TEMPORARY////////////////////	
		m_Mesh = std::make_shared<ParticleMesh>();
		Ref<Material> material = Material::Create(AssetManager::Get()->GetShader("ParticleShader"));
		material->Set(RenderMode::TRANSLUCENT | RenderMode::ADDITIVE);
		MaterialStack::Get()->RegisterMaterial("Particle", material);
		Dot::ECSManager::Get()->AddComponent(entity, RenderComponent{
				m_Mesh,
				MaterialStack::Get()->GetMaterialID("Particle"),
				D_TRIANGLES
			});


		Dot::ECSManager::Get()->AddComponent(entity, Dot::Transform{});
		////////////////////////////////////////////



		Dot::ECSManager::Get()->AddComponent(entity, ComputeComponent{
			AssetManager::Get()->GetShader("ParticleComputeShader")
			});

		Dot::ECSManager::Get()->AddComponent(entity, ParticleEmitter{
				20,  // count
				100, // max count
				1.0f,// speed
				0.0f,// gravity
				5.0f,// min life span
				5.0f // max life span
			});
	
		m_Emitter = &ECSManager::Get()->GetComponent<ParticleEmitter>(entity);
		m_ParticleSystem = ECSManager::Get()->GetSystem<ParticleSystem>();
		m_Mesh->SetRenderCount(m_Emitter->count);
		
		ECSManager::Get()->SaveEntity(entity);
	

		
	}
	void ParticleEditor::OnUpdate()
	{
		
	}
	void ParticleEditor::OnEvent(Event& event)
	{
		
	}
	void ParticleEditor::OnDestroy()
	{
	}
}