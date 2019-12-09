#pragma once
#include "Dot/Graphics/Renderer/Texture.h"
#include "Dot/Graphics/Renderer/Camera/OrthoCamera.h"
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"


namespace Dot {
	
	struct Quad
	{
		Quad(const glm::vec2& position, const glm::vec2& size)
		{
			m_Vertices[0] = glm::vec2(position.x, position.y);
			m_Vertices[1] = glm::vec2(position.x + size.x, position.y);
			m_Vertices[2] = glm::vec2(position.x + size.x, position.y + size.y);
			m_Vertices[3] = glm::vec2(position.x, position.y + size.y);

		}
		glm::vec2 m_Vertices[4];
	};

	class Widget
	{
	public:
		virtual bool MouseHoover(const glm::vec2& mousePos) = 0;
		virtual void Move(const glm::vec2 pos) = 0;
		virtual void SetPosition(const glm::vec2& pos) = 0;
		virtual void ClickHandle() = 0;
		virtual void SetIndex(const unsigned int index) = 0;
		virtual const glm::vec2& GetPosition() = 0;
		virtual const glm::vec2& GetSize() = 0;
		virtual const glm::vec2& GetLabelSize() = 0;
	};

	class Wrapper
	{
	public:
		Wrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size);
		
		void AddWidget(const std::string& label, Ref<Widget> widget, unsigned int index);
		bool MouseHoover(const glm::vec2& mousePos);
		bool MouseResize(const glm::vec2& mousePos);

		void Resize(const glm::vec2& mousePos);
		void Move(const glm::vec2& pos);
		void SetPosition(const glm::vec2& pos);
		
		void SetWidgetPosition();
		void SetIndex(const unsigned int index) { m_Index = index; }



		const glm::vec2& GetPosition() { return m_Position; }
		Widget& GetWidget(const std::string& label) { return *m_Widget[label]; }
		const std::unordered_map <std::string, Ref<Widget> >& const GetWidgets() { return m_Widget; }
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);

	private:
		glm::vec4 GetCoords();
	private:
		std::unordered_map<std::string, Ref<Widget>> m_Widget;

	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;
		unsigned int m_Index;
		bool m_Resizing = false;
	};

	class Gui
	{
	public:
		static void Init(const std::string& texturePack);
		static void AddWidget(const std::string& label, Ref<Widget> widget, const Quad& quad, glm::vec2* texcoord,int num = 4);
		static void AddWrapper(const std::string label, Ref<Wrapper> wrapper, const Quad& quad, glm::vec2* texcoord, int num = 4);
		static void EnableWrapper(const std::string& label);
		static void DisableWrapper();

		static bool HandleLeftClick();
		static bool HandleRightClick();
		static bool HandleMiddleClick();
		static void HandleRelease();

		static void UpdatePosBuffer(unsigned int index, unsigned int size, const void* vertices);
		static void UpdateTexBuffer(unsigned int index, unsigned int size, const void* texcoords);

		static void Update();
		static void BindTexture() { s_Texture->Bind(0); };


		const static Ref<ArrayBuffer>& GetVAO() { return s_VAO; }
		static const Widget& GetWidget(const std::string& label) { return *s_Widget[label]; }
		static const Widget& GetWrappeWidget(const std::string& wrapper,const std::string widget) { return s_Wrapper[wrapper]->GetWidget(widget); }
	private:
		static Ref<Texture>		s_Texture;
		static Ref<ArrayBuffer> s_VAO;

	private:
		static std::unordered_map <std::string, Ref<Widget>>  s_Widget;
		static std::unordered_map <std::string, Ref<Wrapper>> s_Wrapper;
		
	private:
		static std::vector<glm::vec2>  s_Vertice;
		static std::vector <glm::vec2> s_TexCoord;

		static unsigned int s_NumWidgets;
		static std::string	s_AttachedWidget;
		static std::string	s_AttachedWrapper;
		static std::string  s_ResizedWrapper;
		static std::string	s_EnabledWrapper;

	private:
		static glm::vec2 s_MousePosition;
		static bool s_Locked;
	
	};
}


