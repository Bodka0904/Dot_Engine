#pragma once
#include <glm/glm.hpp>

#include "DGui.h"

namespace Dot {
	class DSlider : public DWidget
	{
	public:
		DSlider(const std::string& label, const glm::vec2& position, const glm::vec2& size,float * value);
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void SetPosition(const glm::vec2& pos) override;
		virtual void ClickHandle() override;
		virtual void SetIndex(const unsigned int index) override { m_Index = index; };
		virtual const glm::vec2& GetPosition() override { return m_Position; }

		const int GetIndex() const { return m_Index; }
		const glm::vec2& GetSize() { return m_Size; }

		static DSlider& Get(const std::string& label);
		static DSlider& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float *val);
	private:
		glm::vec4 GetCoords();
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Size;

		float m_TexOffset = 0.0f;
		float m_TempStorage = 0.0f;
		float* m_Value;

		unsigned int m_Index = 0;
	};
}