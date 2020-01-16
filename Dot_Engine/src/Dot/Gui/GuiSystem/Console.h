#pragma once

#include "Dot/Core/Events/Event.h"
#include "Dot/Utils/Text/Text.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Renderer2D.h"

namespace Dot {
	class Console
	{
	public:
		struct CommandLine
		{
			QuadVertex Quad;
			unsigned int Index;
			Ref<Text> Command;
			glm::vec2 Position;
			glm::vec2 Size;
		};

	public:
		Console(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize, float textSize);
		~Console();
		bool MouseHoover(const glm::vec2& mousePos);

		void RegisterCommand(const std::string& cmd,std::function<void()> func);
		void Render(const Ref<Shader>& textShader, const Ref<OrthoCamera> camera);
		void Move(const glm::vec2& pos);
		void SetPosition(const glm::vec2& pos);
		void SetTextColor(const glm::vec3& color) { m_TextColor = color; }
		void Clear();
		void ClickHandle();
		void SubmitCommand();
		void PushText(const std::string& text);
		void TakeInput(KeyPressedEvent& event);
		const bool GetClicked() { return m_Clicked; }

		const glm::vec2& GetPosition() { return m_Position; }
		unsigned int GetIndex() { return m_Index; }
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize = 0.2f, float textSize = 0.18f);
	private:
		glm::vec4 getCoords();
	private:
		CommandLine m_CmdLine;
		QuadVertex m_Quad;
		Ref<Renderer2D> m_TextRenderer;
		Ref<Text> m_Label;
		Ref<Text> m_Text;

		glm::vec2 m_Position;
		glm::vec2 m_Size;
		glm::vec3 m_TextColor;

		unsigned int m_Index;

		bool m_Clicked;
		int m_TexOffset = 0;

	private:
		std::unordered_map<std::string, std::function<void()>> m_Command;
	};
}