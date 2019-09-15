#pragma once
#include <glm/glm.hpp>


#include "Dot/Graphics/Transform.h"
#include "Dot/Graphics/Text/Text.h"
#include "Dot/Graphics/Renderer/Texture.h"
#include "Dot/Graphics/Renderer/Camera/OrthoCamera.h"
#include "Dot/Graphics/Shaders/Shader.h"
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"



namespace Dot {
	struct Vertex
	{
		glm::vec2 position;
		glm::vec2 texcoord;
	};
	struct Quad
	{
		Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec2* texcood)
		{

			m_Vertices[0].position = glm::vec2(position.x, position.y);
			m_Vertices[0].texcoord = texcood[0];
			m_Vertices[1].position = glm::vec2(position.x + size.x, position.y);
			m_Vertices[1].texcoord = texcood[1];
			m_Vertices[2].position = glm::vec2(position.x + size.x, position.y + size.y);
			m_Vertices[2].texcoord = texcood[2];
			m_Vertices[3].position = glm::vec2(position.x, position.y + size.y);
			m_Vertices[3].texcoord = texcood[3];


			m_Indices[0] = 0;
			m_Indices[1] = 1;
			m_Indices[2] = 2;
			m_Indices[3] = 0;
			m_Indices[4] = 3;
			m_Indices[5] = 2;
		}

		Vertex m_Vertices[4];
		unsigned int m_Indices[6];
	};
	class Widget
	{
	public:
		Widget() {}
		virtual ~Widget() {};

		virtual void Render(const Ref<Shader>& shader) = 0;
		virtual void RenderLabel() = 0;
		virtual void ClickHandle() = 0;
		virtual void SetPosition(const glm::vec2& pos) = 0;
		virtual bool MouseHoover(const glm::vec2& mousePos) = 0;

		
		virtual const bool& GetClicked() const = 0;
		virtual const bool& LeftClicked() const = 0;
		virtual const bool& RightClicked() const = 0;
		virtual const glm::vec2& GetPosition() = 0;

		
	};
	class Wrapper
	{
	public:
		Wrapper(const std::string& label, const glm::vec2& position, const glm::vec2& size);
		virtual ~Wrapper();


		void Render(const Ref<Shader>& shader);
		void RenderLabels(const Ref<Shader>& shader);
		void SetPosition(const glm::vec2& pos);
		void ClickHandle();
		bool MouseHoover(const glm::vec2& mousePos);

		void AddWidget(const std::string& label, Ref<Widget> widget) { m_Widget[label] = widget; };
		
		std::unordered_map<std::string, Ref<Widget>>& GetWidgets() { return m_Widget; }
		const glm::vec2& GetPosition() { return m_Transform.GetPos(); }

		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);
	private:
		glm::vec4 GetCoords();

	private:
		Ref<ArrayBuffer>m_VAO;
		Ref<Text>m_Label;
		Transform2D m_Transform;

		glm::vec2 m_Size;

	private:
		std::unordered_map<std::string, Ref<Widget>> m_Widget;
	};

	class WidgetStack
	{
	public:
		WidgetStack(const std::string& widgetShader, const std::string& textShader, const std::string& texturePack);

		static void AddWidget(const std::string& label,Ref<Widget> widget);
		static void AddWrapper(const std::string& label, Ref<Wrapper> wrapper);

		bool HandleLeftClick();
		bool HandleRightClick();
		
	
		void Release();
		void Update();

		void RenderWidgets();
		void RenderLabels();
		void UpdateCamera(int width, int height) { m_Camera->SetProjectionMatrix(0, width, height, 0); };
		
		static void EnableWrapper(const std::string& wrapper);
		static void DisableWrapper();

		static Ref<Widget>& GetWidget(const std::string& name) { return m_Widget[name]; }
		static Ref<Wrapper>& GetWrapper(const std::string& name) { return m_Wrapper[name]; }

	private:
		static std::unordered_map<std::string, Ref<Widget>> m_Widget;
		static std::unordered_map<std::string, Ref<Wrapper>> m_Wrapper;
		
		std::string m_SelectedWidget;

		static std::string m_EnabledWrapper;
	private:
		Ref<Shader>m_Shader;
		Ref<Shader>m_TextShader;
		Ref<OrthoCamera> m_Camera;

		Ref<Texture> m_Texture;
	};
}