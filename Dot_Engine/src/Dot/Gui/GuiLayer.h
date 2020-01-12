#pragma once
#include "Dot/Core/Layer.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shader/Shader.h"

#include "Dot/Renderer/Renderer2D.h"
#include "Dot/Utils/Text/Text.h"

#include "Dot/ParticleEngine/ParticleEditorLayer.h"
namespace Dot {

	class GuiLayer : public Layer
	{
	public:
		GuiLayer();
		~GuiLayer();
		
		void OnAttach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event)override;

	private:
		Ref<OrthoCamera> m_Camera;
		Ref<Shader> m_GuiShader;
		Ref<Shader> m_TextShader;

		ParticleEditorLayer* layer;
	};
}
