#pragma once
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"
#include "Font.h"


namespace Dot {
	class Text
	{
	public:
		Text(Font& font, std::string text, const glm::vec2& position ,const glm::vec2& size);
		void RenderText();

	private:
		Ref<ArrayBuffer>m_VAO;

	private:
		std::vector <float> m_Vertice;
		std::vector <float> m_TexCoord;
	};
}