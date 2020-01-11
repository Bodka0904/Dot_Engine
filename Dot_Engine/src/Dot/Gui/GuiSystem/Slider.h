#pragma once
#include <glm/glm.hpp>

#include "Dot/Utils/Text/Text.h"
#include "Gui.h"

namespace Dot {
	class Slider : public Widget
	{
	public:
		Slider(const glm::vec2& position, const glm::vec2& size,float * value,float range,float labelsize = 0.2f);
		virtual bool MouseHoover(const glm::vec2& mousePos) override;
		virtual void UpdateData() override;
		virtual void ClickHandle() override;
		virtual void Exit()override;
		virtual void SetLabel(const Ref<Text> label) override;
		virtual void Move(const glm::vec2 pos) override { m_Position += pos; };
		virtual void SetIndex(const unsigned int index) override { m_Index = index; };
		virtual void SetPosition(const glm::vec2& pos) override { m_Position = pos; };
		void SetTextHandle(const Ref<Text> text);

		virtual const glm::vec2& GetPosition() override { return m_Position; }
		virtual const glm::vec2& GetSize() override { return m_Size; }
		virtual const glm::vec2& GetLabelSize() override;

		virtual const unsigned int GetIndex() override { return m_Index; }

		static Slider& Get(const std::string& label);
		static Slider& GetWrapped(const std::string& wrapper, const std::string& label);
		static void Create(const std::string& label, const glm::vec2& position, const glm::vec2& size,float *val,float range, float labelsize = 0.2f);
	private:
		glm::vec4 getCoords();
	private:
		Ref<Text> m_Label;
		Ref<Text> m_Text;

		glm::vec2 m_Position;
		glm::vec2 m_Size;

		float m_TexOffset = 0.0f;
		float m_TempStorage = 0.0f;
		float m_Range = 0.0f;
		float* m_Value;

		unsigned int m_Index = 0;
	};
}