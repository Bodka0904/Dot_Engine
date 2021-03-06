#pragma once
#include "Dot/Core/Events/Event.h"
#include "Dot/Utils/Text/Text.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Renderer2D.h"


namespace Dot {
	class ConsoleText
	{
		friend class Console;
	public:
		ConsoleText(const glm::vec2& position, const glm::vec2& size);

		void Push(std::string& text,Ref<Renderer2D>& renderer, const glm::vec3& color);
		void SetSize(const glm::vec2& size);
	private:
		Text Text;
		glm::vec2 Size;
		float CharSize;
		float LineHeight;
	};


	class Console
	{
	public:
		struct CommandLine
		{
			QuadVertex2D Quad;
			unsigned int Index;
			Ref<Text> Command;
			glm::vec2 Position;
			glm::vec2 Size;
		};
	public:
		Console(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label);
		~Console();
		void Clean();
		bool MouseHoover(const glm::vec2& mousePos);
		void RegisterCommand(const std::string& cmd, std::function<void()> func);
		void Render();

		void SetPosition(const glm::vec2& pos);
		void Move(const glm::vec2& pos);
		void PushText(std::string& text, const glm::vec3& color);
		bool OnLeftClick(const glm::vec2& mousePos);

		void SetX(float pos, float size);
		void SetY(float pos, float height);

		void SubmitCommand();
		bool TakeInput(KeyPressedEvent& event);
		const bool Clicked() { return m_Clicked; }

		static Ref<Console> Create(const glm::vec2& position, const glm::vec2& size, const glm::vec3& labelColor, const std::string& label);
	private:
		glm::vec4 getCoords();
		void updateBuffers();
		void clearConsole();
	private:
		Ref<Renderer2D> m_TextRenderer;
		Ref<ConsoleText> m_Text;
		CommandLine m_CmdLine;

		Text m_Label;
		QuadVertex2D m_Quad;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		int m_Index;
		bool m_Clicked;
	private:
		std::unordered_map<std::string, std::function<void()>> m_Command;
	};
}