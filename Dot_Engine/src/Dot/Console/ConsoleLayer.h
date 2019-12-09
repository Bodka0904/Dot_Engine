#pragma once
#include "Dot/Layer.h"
#include "Dot/Graphics/Console/Console.h"
#include "Dot/Graphics/Renderer/Camera/OrthoCamera.h"
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Renderer2D.h"

namespace Dot {
	class ConsoleLayer : public Layer
	{
	public:
		ConsoleLayer();
		~ConsoleLayer();

		void OnAttach() override;
		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event)override;

	private:
		Ref<Console> m_Console;
		Ref<Renderer2D> m_Renderer;
		Ref<OrthoCamera> m_Camera;
		Ref<Shader> m_Shader;
		Ref<Shader> m_TextShader;

	};
}