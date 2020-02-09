#pragma once
#include <glm/glm.hpp>
#include "Dot/Renderer/Renderer2D.h"

namespace Dot {
	
	class Widget
	{
	public:
		virtual bool MouseHoover(const glm::vec2& mousePos) = 0;
		virtual void Move(const glm::vec2& pos) = 0;
		virtual void SetPosition(const glm::vec2& pos) = 0;
		virtual bool Release() { return false; };

		virtual void SetColor(const glm::vec3& color) = 0;
		virtual void SetSize(const glm::vec2& size) = 0;
		virtual void StopRender() = 0;
		virtual bool OnLeftClick(const glm::vec2& mousePos) { return false; };
		virtual void Clean() = 0;
		virtual const glm::vec2& GetSize() = 0;

	};
}