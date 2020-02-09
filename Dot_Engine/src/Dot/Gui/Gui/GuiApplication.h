#pragma once
#include "GuiBlock.h"
#include "Dot/Core/Events/Event.h"

#include "Dot/Utils/Text/Text.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Renderer2D.h"

#include <array>
#include <queue>
#include <set>

namespace Dot {

#define MAX_QUADS 500
#define MAX_TEXT_CHAR 32
#define MAX_CHAR_PER_LABEL 32
#define MAX_CHAR_PER_CONSOLE 2000



	struct GuiRenderLayer
	{
		GuiRenderLayer()
		{
			for (int i = 0; i < MAX_QUADS; ++i)
			{
				availableIndex.push(i);
			}
			guiRenderer = std::make_shared<Renderer2D>(MAX_QUADS);
			labelRenderer = std::make_shared<Renderer2D>(MAX_QUADS * MAX_CHAR_PER_LABEL);
			textRenderer = std::make_shared<Renderer2D>(MAX_QUADS * MAX_TEXT_CHAR);
		}

		Ref<Renderer2D> guiRenderer;
		Ref<Renderer2D> labelRenderer;
		Ref<Renderer2D> textRenderer;

		std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> availableIndex{};
		unsigned int numQuads = 0;
	};
	
	class GuiApplication
	{
	public:
		// TODO probably change in the future to be able to add layers
		enum
		{
			BACK,
			FRONT,
			NUM_GUI_LAYERS
		};

	public:
		GuiApplication(const GuiApplication&) = delete;
		~GuiApplication();

		void Update();
		void OnEvent(Event& e);
		unsigned int AddBlock(Ref<GuiBlock>);
		void SwitchBlock(unsigned int ID);
		void PushIndex(int index,int layer = BACK);
		int PopIndex(int layer = BACK);

		void UpdateVertexBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len = 1, int layer = BACK);
		void UpdateTextBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len, int layer = BACK);
		void UpdateLabelBuffer(unsigned int index, const QuadVertex2D* vertices, unsigned int len, int layer = BACK);


		static void Init(const std::string& texturePath);
		Ref<GuiBlock>& GetCurrent() { return m_CurrentBlock; }
		static GuiApplication* Get() { return s_Instance; }

	private:
		GuiApplication(const std::string& texturePath);

		Ref<Texture2D> m_Texture;
		Ref<OrthoCamera> m_Camera;
		Ref<Shader> m_GuiShader;


		GuiRenderLayer m_Layers[NUM_GUI_LAYERS];

		QuadVertex2D m_BackGroundQuad;
		glm::vec2 m_WindowSize;
	private:
		unsigned int m_InsertedBlockID;
		std::unordered_map<unsigned int, Ref<GuiBlock>> m_Blocks;
		Ref<GuiBlock> m_CurrentBlock;

		static GuiApplication* s_Instance;
	};
}