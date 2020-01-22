#pragma once
#include "GuiBlock.h"
#include "Dot/Core/Events/Event.h"

#include "Dot/Utils/Text/Text.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Renderer2D.h"


#include <queue>

namespace Dot {

#define MAX_QUADS 500
#define MAX_TEXT_CHAR 32
#define MAX_CHAR_PER_LABEL 32
#define MAX_CHAR_PER_CONSOLE 2000

	class GuiApplication
	{
	public:
		GuiApplication(const std::string& texturePath);
		~GuiApplication();

		void Update();
		void PushBlock(GuiBlock* block);
		void PopBlock(GuiBlock* block);
		void OnEvent(Event& e);
		void PushIndex(unsigned int index);
		unsigned int PopIndex();

		void UpdateVertexBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len = 1);
		void UpdateTextBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len);
		void UpdateLabelBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len);

		static void Init(const std::string& texturePath);
		static GuiApplication* Get() { return s_Instance; }
	private:
		std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_AvailableIndex{};
		unsigned int m_NumQuads = 1;
	private:
		Ref<Texture2D> m_Texture;
		Ref<OrthoCamera> m_Camera;
		Ref<Shader> m_GuiShader;

		Ref<Renderer2D> m_GuiRenderer;
		Ref<Renderer2D> m_LabelRenderer;
		Ref<Renderer2D> m_TextRenderer;

		QuadVertex2D m_BackGroundQuad;
		glm::vec2 m_WindowSize;

	private:
		std::vector<GuiBlock*> m_Blocks;

		unsigned int m_BlockInsertIndex = 0;

		static GuiApplication* s_Instance;
	};
}