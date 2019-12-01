#pragma once
#include <glm/glm.hpp>
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"

#include "DGui.h"

namespace Dot {

	class DCheckbox : public DWidget
	{
	public:
		DCheckbox(const std::string& label, const glm::vec2& position, const glm::vec2& size);
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void SetPosition(const glm::vec2& pos) override;
		virtual void ClickHandle() override;
		virtual void SetIndex(const unsigned int index) override { m_Index = index; };
		virtual const glm::vec2& GetPosition() override { return m_Position; }

		const int GetIndex() const { return m_Index; }
		const bool& GetClicked() const { return m_Clicked; }
		const glm::vec2& GetSize() { return m_Size; }

		static DCheckbox& Get(const std::string& label);
		static DCheckbox& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size);
	private:
		glm::vec4 GetCoords();
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;

		bool m_Clicked = false;
		int m_TexOffset = 0;
		unsigned int m_Index = 0;

	};
}