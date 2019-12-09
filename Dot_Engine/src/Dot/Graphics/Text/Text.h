#pragma once
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Camera/OrthoCamera.h"
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"


namespace Dot {
	class Text
	{
	public:
		Text(const std::string& font, std::string text, const glm::vec2 position ,const glm::vec2 size);
		void SetPosition(const glm::vec2& position);
		const glm::vec2& GetSize() const { return m_Size; }

		static const Ref<ArrayBuffer>& GetVAO() { return s_VAO; }
		static void Init();

	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;
		unsigned int m_Len = 0;
		unsigned int m_PositionInBuffer = 0;

	private:
		static Ref<ArrayBuffer> s_VAO;
	private:
		static std::vector <glm::vec2> s_Vertice;
		static std::vector <glm::vec2> s_TexCoord;
		static unsigned int			   s_NumChars;
	};


}