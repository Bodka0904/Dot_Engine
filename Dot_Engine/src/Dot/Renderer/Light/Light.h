#pragma once
#include <glm/glm.hpp>

namespace Dot {
	class Light
	{
	public:
		Light(const glm::vec3& position, const glm::vec3& color);

		void SetPosition(const glm::vec3& position);
		void SetColor(const glm::vec3& color);
		void SetStrength(const float& strength);

		glm::vec3& GetPosition() { return m_Position; }
		glm::vec3& GetColor() { return m_Color; }
		float& GetStrength() { return m_Strength; }
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Color;
		float m_Strength = 0.7f;

	};

}