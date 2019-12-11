#pragma once
#include "Dot/Core/Layer.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shaders/Shader.h"

#include "Dot/Utils/Text/DynamicText.h"

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
		float value = 0.0f;
		Ref<OrthoCamera> m_Camera;
		Ref<Shader> m_GuiShader;
		Ref<Shader> m_TextShader;
		Ref<Shader> m_TestShader;

	private:
		Ref<DynamicText> m_Text;

	};
}
