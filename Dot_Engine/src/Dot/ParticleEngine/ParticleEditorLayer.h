#pragma once
#include "Dot/Core/Layer.h"
#include "ParticleEffect.h"
#include "Dot/ECS/ECSManager.h"
#include "Dot/Renderer/Renderable.h"

namespace Dot {
	class ParticleEditorLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event)override;

		virtual void OnGuiAttach() override;
		virtual void OnGuiUpdate() override;
		virtual void OnGuiDetach() override;
		virtual void OnGuiEvent(Event& event) override;

		std::pair<std::string, Entity> GetEntity();
	private:
		Ref<ParticleEffect> m_Effect;
		Ref<Material> m_Material;

		ParticleComponent* m_ParComponent;
		Transform* m_TransformComponent;
		std::pair<std::string, Entity> m_Entity;
	};
}