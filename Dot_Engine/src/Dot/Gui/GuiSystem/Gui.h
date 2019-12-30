#pragma once
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shaders/Shader.h"
#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/Utils/Text/StaticText.h"
#include "Dot/Renderer/Renderer2D.h"

namespace Dot {
	
	class Widget
	{
	public:
		virtual bool MouseHoover(const glm::vec2& mousePos) = 0;
		virtual void Move(const glm::vec2 pos) = 0;
		virtual void SetPosition(const glm::vec2& pos) = 0;
		virtual void ClickHandle() = 0;
		virtual void SetIndex(const unsigned int index) = 0;
		virtual void Minimize() = 0;
		virtual const glm::vec2& GetPosition() = 0;
		virtual const glm::vec2& GetSize() = 0;
		virtual const glm::vec2& GetLabelSize() = 0;
	};

	class Wrapper
	{
	public:
		Wrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize = 0.2);
		
		void AddWidget(const std::string& label, Ref<Widget> widget, unsigned int index);
		bool MouseHoover(const glm::vec2& mousePos);
		bool MouseResize(const glm::vec2& mousePos);
		bool Minimize(const glm::vec2& mousePos);
		
		void Resize(const glm::vec2& mousePos);
		void Move(const glm::vec2& pos);
		void SetPosition(const glm::vec2& pos);
		
		void SetWidgetPosition();
		void SetIndex(const unsigned int index) { m_Index = index; m_ExitButton.SetIndex(index + 1); }


		const bool IsMinimized() const { return m_Minimized; }
		const glm::vec2& GetPosition() { return m_Position; }
		Widget& GetWidget(const std::string& label) { return *m_Widget[label]; }
		const std::unordered_map <std::string, Ref<Widget> >& const GetWidgets() { return m_Widget; }
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);

	private:
		class ActionButton
		{
		public:
			ActionButton(const glm::vec2& position, const glm::vec2& size);
			bool MouseHoover(const glm::vec2& mousePos);
			void Move(const glm::vec2& pos);
			void SetPosition(const glm::vec2& pos);
			void SetSize(const glm::vec2& size) { m_Size = size; };
			void SetIndex(unsigned int index) { m_Index = index; }

			const glm::vec2& GetPosition() { return m_Position; }
			const glm::vec2& GetSize() { return m_Size; }
 		private:
			glm::vec4 GetCoords();
		private:
			glm::vec2 m_Position;
			glm::vec2 m_Size;

			unsigned int m_Index;
		};

		ActionButton m_ExitButton;
		StaticText m_Label;
	private:
		glm::vec4 GetCoords();
	private:
		std::unordered_map<std::string, Ref<Widget>> m_Widget;

	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;
		unsigned int m_Index;
		bool m_Resizing = false;
		bool m_Minimized = false;
	};

	class Gui
	{
	public:	
		Gui(const std::string& texturePack);
		~Gui();
		void AddWidget(const std::string& label, Ref<Widget> widget, const QuadVertex* quad,int num = 1);
		void AddWrapper(const std::string label, Ref<Wrapper> wrapper, const QuadVertex* quad, int num = 2);
		void EnableWrapper(const std::string& label);
		void DisableWrapper();

		bool HandleLeftClick();
		bool HandleRightClick();
		
		void HandleRelease();
		void UpdatePosBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len = 1);
		void UpdateTexBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len = 1);
		void Update();
		void Render(const Ref<Shader> shader,const Ref<OrthoCamera> camera);
		
		const Widget& GetWidget(const std::string& label) { return *m_Widget[label]; }
		const Widget& GetWrappedWidget(const std::string& wrapper,const std::string widget) { return m_Wrapper[wrapper]->GetWidget(widget); }
		
		static void Init(const std::string& texturePack);
		static Gui* Get() {return s_Instance;}
	private:
		Ref<Texture2D>		m_Texture;
		Ref<Renderer2D>		m_GuiRenderer;
	private:
		std::unordered_map <std::string, Ref<Widget>>  m_Widget;
		std::unordered_map <std::string, Ref<Wrapper>> m_Wrapper;
		
	private:
		std::vector<QuadVertex> m_Vertices;

		unsigned int m_NumWidgets;
		std::string	 m_AttachedWidget;
		std::string	 m_AttachedWrapper;
		std::string  m_ResizedWrapper;
		std::string	 m_EnabledWrapper;

	private:
		glm::vec2 m_MousePosition;
		bool	  m_Locked;

	private:
		static Gui* s_Instance;

	};
}


