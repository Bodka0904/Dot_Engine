#pragma once
#include <glm/glm.hpp>

#include "DGui.h"


namespace Dot {
	class DArrbutton : public DWidget
	{
	public:
		DArrbutton(const std::string& label, const glm::vec2& position, const glm::vec2& size);
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void SetPosition(const glm::vec2& pos) override;
		virtual void ClickHandle() override { *m_Clicked = !*m_Clicked; }
		virtual void SetIndex(const unsigned int index) override { m_Index = index; };
		virtual const glm::vec2& GetPosition() override { return m_Position; }

		const bool& LeftClicked() const { return m_LeftClick; }
		const bool& RightClicked() const { return m_RightClick; }

		const int GetIndex() const { return m_Index; }
		const glm::vec2& GetSize() { return m_Size; }

		static DArrbutton& Get(const std::string& label);
		static DArrbutton& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);
	private:
		glm::vec4 GetCoords();
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;

		bool * m_Clicked = false;
		bool m_LeftClick = false;
		bool m_RightClick = false;
		unsigned int m_Index = 0;

	};
}