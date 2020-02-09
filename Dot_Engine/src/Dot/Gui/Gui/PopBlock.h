#pragma once
#include "Dot/Core/Events/Event.h"
#include "Dot/Renderer/Renderer2D.h"
#include "Widgets/Widget.h"
#include "Widgets/GuiWindow.h"

namespace Dot {

	class PopBlock
	{
	public:
		PopBlock(const glm::vec2& position, const glm::vec2& size,const std::string& name);
		

		void Render(const Ref<Shader>& shader,const Ref<Texture2D>& texture);

	private:
		Ref<Renderer2D> m_Renderer;
		Ref<Renderer2D> m_TextRenderer;
	};

}