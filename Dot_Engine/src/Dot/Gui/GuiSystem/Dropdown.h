#pragma once
#include <glm/glm.hpp>

#include "Dot/Utils/Text/Text.h"
#include "Gui.h"

namespace Dot {
	class Dropdown : public Widget
	{
	public:
		struct Dropbox
		{
			QuadVertex Quad;
			Ref<Text> Label;
			glm::vec2 Position;
			glm::vec2 Size;
			bool Clicked;
			unsigned int Index;
		};
	public:
		Dropdown(const std::string& label, const glm::vec2& position, const glm::vec2& size, float labelsize = 0.18f);
		~Dropdown();
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void ClickHandle() override;
		virtual void Minimize()override;
		virtual void Move(const glm::vec2 pos) override;
		virtual void SetPosition(const glm::vec2& pos)override;

		virtual const glm::vec2& GetPosition() override { return m_Box[0].Position; }
		virtual const glm::vec2& GetSize() override { return m_Box[0].Size; }
		virtual const glm::vec2& GetLabelSize() override;

		void AddDropbox(const std::string& label,float labelsize = 0.18f);
		const bool GetBoxClicked(int index);
		const bool GetClicked();
		virtual const unsigned int GetIndex() override { return m_Box[0].Index; }

		static Dropdown& Get(const std::string& label);
		static Dropdown& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float labelsize = 0.18f);
	private:
		glm::vec4 getCoords();
	private:
	
		std::vector<Dropbox> m_Box;

		bool m_Clicked = false;
		int m_ClickedIndex = 0;
		static constexpr float s_BoxOffset = 1.0f;
	};
}