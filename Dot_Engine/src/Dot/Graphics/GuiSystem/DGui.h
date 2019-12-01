#pragma once
#include "Dot/Graphics/Renderer/Texture.h"
#include "Dot/Graphics/Renderer/Camera/OrthoCamera.h"
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"

namespace Dot {
	
	struct DQuad
	{
		DQuad(const glm::vec2& position, const glm::vec2& size)
		{
			m_Vertices[0] = glm::vec2(position.x, position.y);
			m_Vertices[1] = glm::vec2(position.x + size.x, position.y);
			m_Vertices[2] = glm::vec2(position.x + size.x, position.y + size.y);
			m_Vertices[3] = glm::vec2(position.x, position.y + size.y);

		}
		glm::vec2 m_Vertices[4];
	};

	class DWidget
	{
	public:
		virtual bool MouseHoover(const glm::vec2& mousePos) = 0;
		virtual void SetPosition(const glm::vec2& pos) = 0;
		virtual void ClickHandle() = 0;
		virtual void SetIndex(const unsigned int index) = 0;
		virtual const glm::vec2& GetPosition() = 0;
	};

	class DWrapper
	{
	public:
		DWrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size);
		
		void AddWidget(const std::string& label, Ref<DWidget> widget, unsigned int index);
		bool MouseHoover(const glm::vec2& mousePos);
		void SetPosition(const glm::vec2& pos);
		void SetWidgetPosition();
		void SetIndex(const unsigned int index) { m_Index = index; }

		const glm::vec2& GetPosition() { return m_Position; }
		DWidget& GetWidget(const std::string& label) { return *m_Widget[label]; }
		const std::unordered_map <std::string, Ref<DWidget> >& const GetWidgets() { return m_Widget; }
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);
	private:
		glm::vec4 GetCoords();
	private:
		std::unordered_map<std::string, Ref<DWidget>> m_Widget;

	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;
		unsigned int m_Index;


	};

	class DGui
	{
	public:
		static void Init(const std::string& texturePack, const std::string& shader,const std::string & textShader);
		static void AddWidget(const std::string& label, Ref<DWidget> widget, const DQuad& quad, glm::vec2* texcoord,int num = 4);
		static void AddWrapper(const std::string label, Ref<DWrapper> wrapper, const DQuad& quad, glm::vec2* texcoord, int num = 4);
		static void EnableWrapper(const std::string& label);
		static void DisableWrapper();

		static bool HandleLeftClick();
		static bool HandleRightClick();
		static void HandleRelease();

		static void UpdatePosBuffer(unsigned int index, unsigned int size, const void* vertices);
		static void UpdateTexBuffer(unsigned int index, unsigned int size, const void* texcoords);

		static void UpdateCamera(const glm::vec2& window);
		static void Update();
		static void Render();

		static const DWidget& GetWidget(const std::string& label) { return *m_Widget[label]; }
		static const DWidget& GetWrappedWidget(const std::string& wrapper,const std::string widget) { return m_Wrapper[wrapper]->GetWidget(widget); }
	private:
		static Ref<Shader>m_Shader;
		static Ref<Shader>m_TextShader;
		static Ref<OrthoCamera> m_Camera;
		static Ref<Texture> m_Texture;
		static Ref<ArrayBuffer> m_VAO;

	private:
		static std::unordered_map<std::string, Ref<DWidget>> m_Widget;
		static std::unordered_map < std::string, Ref<DWrapper>> m_Wrapper;
		
	private:
		static std::vector<glm::vec2> m_Vertice;
		static std::vector <glm::vec2> m_TexCoord;

		static unsigned int m_NumWidgets;
		static std::string m_AttachedWidget;
		static std::string m_AttachedWrapper;
		static std::string m_EnabledWrapper;
	
	};
}