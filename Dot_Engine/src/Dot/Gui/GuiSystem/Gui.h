#pragma once

#include "Dot/Utils/Text/Text.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/Renderer/Renderer2D.h"

namespace Dot {
#define MAX_QUADS 300
#define MAX_TEXT_CHAR 32
#define MAX_CHAR_PER_LABEL 32

	class Widget
	{
	public:
		virtual bool MouseHoover(const glm::vec2& mousePos) = 0;
		virtual void Move(const glm::vec2 pos) = 0;
		virtual void ClickHandle() = 0;
		virtual void SetPosition(const glm::vec2& pos) = 0;
		virtual void Exit() = 0;
		virtual const glm::vec2& GetPosition() = 0;
		virtual const glm::vec2& GetSize() = 0;
		virtual const glm::vec2& GetLabelSize() = 0;
		virtual const unsigned int GetIndex() = 0;
	};

	class Wrapper
	{
	public:
		Wrapper(const std::string& label,const glm::vec2& position, const glm::vec2& size, int widgetPerCol, bool locked, float labelsize = 0.2);

		void AddWidget(const std::string& label, Ref<Widget> widget,bool start = false);
		void RemoveWidgets();
		bool MouseHoover(const glm::vec2& mousePos);
		bool MouseResize(const glm::vec2& mousePos);
		bool Exit(const glm::vec2& mousePos);

		void Resize(const glm::vec2& mousePos);
		void Move(const glm::vec2& pos);
		void SetPosition(const glm::vec2& pos);
		void SetWidgetPosition();
		

		const glm::vec2& GetPosition() { return m_Position; }
		std::pair<unsigned int, unsigned int>& GetIndex() { return std::pair<unsigned int, unsigned int>(m_Index, m_ExitButton.GetIndex()); }
		Widget& GetWidget(const std::string& label) { return *m_Widget[label]; }
		const std::map <std::string, Ref<Widget> >& GetWidgets() { return m_Widget; }
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, int widgetPerCol, bool locked = false, float labelsize = 0.2f);

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
			unsigned int GetIndex() { return m_Index; }
		private:
			glm::vec4 getCoords();
		private:
			glm::vec2 m_Position;
			glm::vec2 m_Size;

			unsigned int m_Index;
		};

		ActionButton m_ExitButton;
		Ref<Text> m_Label;
	private:
		glm::vec4 getCoords();
	private:
		std::map<std::string, Ref<Widget>> m_Widget;

	private:
		QuadVertex m_Quad[2];
		glm::vec2 m_Position;
		glm::vec2 m_Size;
		
		unsigned int m_Index;

		int m_WidgetPerCol;
		bool m_Resizing = false;
		bool m_Locked = false;
	};

	class Gui
	{
	public:
		Gui(const std::string& texturePack);
		~Gui();
		void AddWrapper(const std::string label, Ref<Wrapper> wrapper);
		void AddWidget(const std::string& label, Ref<Widget> widget, bool start = false);
		void RemoveWidget(const std::string& label);
		void RemoveWrapper(const std::string& label);

		int PopIndex();
		void EnableWrapper(const std::string& label);
		void DisableWrapper();

		bool HandleLeftClick();
		bool HandleRightClick();
		void HandleRelease();

		void UpdateVertexBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len = 1);
		void UpdateTextBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len);
		void UpdateLabelBuffer(unsigned int index, const QuadVertex* vertices, unsigned int len);


		void SetLabelColor(const glm::vec3& color) { m_LabelColor = color; }
		void SetTextColor(const glm::vec3& color) { m_TextColor = color; }

		void Update();
		void Render(const Ref<Shader>& shader, const Ref<Shader>& textShader, const Ref<OrthoCamera> camera);

		const Widget& GetWidget(const std::string& label) { return *m_Widget[label]; }
		const Widget& GetWrappedWidget(const std::string& wrapper, const std::string widget) { return m_Wrapper[wrapper]->GetWidget(widget); }
		Wrapper& GetWrapper(const std::string& label) { return *m_Wrapper[label]; }

		static void Init(const std::string& texturePack);
		static Gui* Get() { return s_Instance; }
	private:
		Ref<Renderer2D>		m_GuiRenderer;
		Ref<Renderer2D>		m_TextRenderer;
		Ref<Renderer2D>		m_LabelRenderer;

		Ref<Texture2D>		m_Texture;
	private:
		std::unordered_map <std::string, Ref<Wrapper>> m_Wrapper;
		std::map <std::string, Ref<Widget>>  m_Widget;

	private:
		std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_AvailableIndex{};
		unsigned int m_NumWidgets;

		std::string	 m_AttachedWidget;
		std::string	 m_AttachedWrapper;
		std::string  m_ResizedWrapper;
		std::string	 m_EnabledWrapper;
	private:
		glm::vec2 m_MousePosition;

	private:
		glm::vec3 m_LabelColor;
		glm::vec3 m_TextColor;

	private:
		static Gui* s_Instance;

	};
}

