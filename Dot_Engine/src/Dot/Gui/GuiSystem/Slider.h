#pragma once
#include <glm/glm.hpp>

#include "Dot/Utils/Text/Text.h"
#include "Gui.h"

namespace Dot {
	class Slider : public Widget
	{
	public:
		Slider(const std::string& label,const glm::vec2& position, const glm::vec2& size,float * value,float rangeStart,float rangeEnd,float labelsize);
		~Slider();
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void ClickHandle() override;
		virtual void Minimize()override;
		virtual void Move(const glm::vec2 pos) override;
		virtual void SetPosition(const glm::vec2& pos) override;

		virtual const glm::vec2& GetPosition() override { return m_Position; }
		virtual const glm::vec2& GetSize() override { return m_Size; }
		virtual const glm::vec2& GetLabelSize() override;

		virtual const unsigned int GetIndex() override { return m_Index; }

		static Slider& Get(const std::string& label);
		static Slider& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float *val, float rangeStart, float rangeEnd, float labelsize = 0.18f);
	private:
		glm::vec4 getCoords();
	private:
		Ref<Text> m_Label;
		Ref<Text> m_Text;
		QuadVertex m_Quad;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		float m_TexOffset = 0.0f;
		float m_TempStorage = 0.0f;
		float m_RangeStart = 0.0f;
		float m_RangeEnd;
		float* m_Value;

		unsigned int m_Index = 0;
	};
}