#pragma once

#include "Dot/Utils/Text/Text.h"
#include "Dot/Renderer/Texture.h"
#include "Dot/Renderer/Camera/OrthoCamera.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Renderer/Renderer2D.h"

#include "Console.h"

namespace Dot {
#define MAX_QUADS 400
#define MAX_TEXT_CHAR 32
#define MAX_CHAR_PER_LABEL 32
#define MAX_CHAR_PER_CONSOLE 1000


	class Widget
	{
	public:
		virtual bool MouseHoover(const glm::vec2& mousePos) = 0;
		virtual void Move(const glm::vec2 pos) = 0;
		virtual void ClickHandle() = 0;
		virtual void SetPosition(const glm::vec2& pos) = 0;
		virtual void Minimize() = 0;
		virtual const glm::vec2& GetPosition() = 0;
		virtual const glm::vec2& GetSize() = 0;
		virtual const glm::vec2& GetLabelSize() = 0;
		virtual const unsigned int GetIndex() = 0;
	};

	struct Panel
	{
		QuadVertex Quad[2];
		glm::vec2 Position;
		glm::vec2 Size;
		
		glm::vec2 PositionButton;
		glm::vec2 SizeButton;
		unsigned int Index;
		unsigned int IndexButton;

		Panel() = default;
		Panel(const glm::vec2& pos,const glm::vec2& size)
			:
			Position(pos),
			Size(size),
			SizeButton(glm::vec2(size.y,size.y)),
			PositionButton(glm::vec2(pos.x + size.x - size.y,pos.y)),
			Index(-1),
			IndexButton(-1)
		{
			glm::vec2 texCoordsPanel[8] = {
						 glm::vec2(0.76f, 0.75f),
						 glm::vec2(1.0f,  0.75f),
						 glm::vec2(1.0f,  1.0f),
						 glm::vec2(0.76f, 1.0f),

						 glm::vec2(0.5f, 0.75f),
						 glm::vec2(0.75f,0.75f),
						 glm::vec2(0.75f,1.0f),
						 glm::vec2(0.5f, 1.0f),
			};
			Quad[0] = QuadVertex(Position, Size, &texCoordsPanel[0]);
			Quad[1] = QuadVertex(PositionButton, SizeButton, &texCoordsPanel[4]);
		}

		void SetPosition(const glm::vec2& pos)
		{
			Position = pos;
			PositionButton =  glm::vec2(pos.x + Size.x - SizeButton.y, Position.y);
			Quad[0].SetPosition(Position, Size);
			Quad[1].SetPosition(PositionButton, SizeButton);
		}
		void Move(const glm::vec2& pos)
		{
			Position += pos;
			PositionButton += pos;
			Quad[0].Move(pos);
			Quad[1].Move(pos);
		}
		bool ButtonClicked(const glm::vec2& pos)
		{
			if (pos.y < PositionButton.y + SizeButton.y && pos.x > PositionButton.x)
				return true;
		
			return false;
		}
	};

	class Wrapper
	{
	public:
		Wrapper(const std::string& label,const glm::vec2& position, const glm::vec2& size, int widgetPerCol, bool locked, float labelsize = 0.2);
		~Wrapper();
		void AddWidget(const std::string& label, Ref<Widget> widget,bool start = false);
		bool MouseHoover(const glm::vec2& mousePos);
		bool MouseResize(const glm::vec2& mousePos);

		void Minimize();
		void Resize(const glm::vec2& mousePos);
		void Move(const glm::vec2& pos);
		void SetPosition(const glm::vec2& pos);
		void SetWidgetPosition();
		

		const glm::vec2& GetPosition() { return m_Position; }
		unsigned int GetIndex() { return m_Index; }
		Widget& GetWidget(const std::string& label) { return *m_Widget[label]; }
		const std::map <std::string, Ref<Widget> >& GetWidgets() { return m_Widget; }
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size, int widgetPerCol, bool locked = false, float labelsize = 0.2f);
	
	private:
		glm::vec4 getCoords();
	private:
		std::map<std::string, Ref<Widget>> m_Widget;
		QuadVertex m_Quad;
		Panel m_Panel;
		Ref<Text> m_Label;

		glm::vec2 m_Position;
		glm::vec2 m_Size;
		glm::vec2 m_CachedSize = glm::vec2(0);
		glm::vec2 m_CachedPosition = glm::vec2(0);
		unsigned int m_Index;

		int m_WidgetPerCol;
		bool m_Resizing = false;
		bool m_Locked = false;
		bool m_Minimized = false;
	};

	class Gui
	{
	public:
		Gui(const std::string& texturePack);
		~Gui();
		void AddWrapper(const std::string label, Ref<Wrapper> wrapper);
		void AddWidget(const std::string& label, Ref<Widget> widget, bool start = false);
		void AddConsole(const std::string& label, Ref<Console> console);

		void RemoveWidget(const std::string& label);
		void RemoveWrapper(const std::string& label);
		void RemoveConsole(const std::string& label);

		int PopIndex();
		void PushIndex(unsigned int index);
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
		Console& GetConsole(const std::string& label) { return *m_Console[label]; }
		static void Init(const std::string& texturePack);
		static Gui* Get() { return s_Instance; }
	private:
		Ref<Renderer2D>		m_GuiRenderer;
		Ref<Renderer2D>		m_TextRenderer;
		Ref<Renderer2D>		m_LabelRenderer;

		Ref<Texture2D>		m_Texture;
	private:
		std::unordered_map <std::string, Ref<Wrapper>> m_Wrapper;
		std::unordered_map<std::string, Ref<Console>> m_Console;
		std::map <std::string, Ref<Widget>>  m_Widget;

	private:
		std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_AvailableIndex{};
		unsigned int m_NumQuads;

		std::string	 m_AttachedWidget;
		std::string	 m_AttachedWrapper;
		std::string  m_AttachedConsole;
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

