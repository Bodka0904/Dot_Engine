#pragma once
#include "Dot/Core/Editor.h"
#include "Dot/ECS/ECSManager.h"
#include "ParticleComponent.h"
#include "ParticleSystem.h"
#include "Dot/Renderer/Renderable/ParticleMesh.h"

namespace Dot{

	class ParticleEditor : public Editor
	{
	public:
		virtual void OnCreate(Entity entity);
		virtual void OnUpdate();
		virtual void OnEvent(Event& event);
		virtual void OnDestroy();

	private:
		Entity m_Entity;
		ParticleEmitter* m_Emitter;
		Ref<ParticleSystem> m_ParticleSystem;
		Ref<ParticleMesh> m_Mesh;

		float test = 2.0f;
	};
}