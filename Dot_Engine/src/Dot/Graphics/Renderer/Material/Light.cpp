#include "stdafx.h"
#include "Light.h"

Dot::Light::Light(const glm::vec3& position, const glm::vec3& color)
	:m_Color(color),m_Position(position)
{
}

void Dot::Light::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void Dot::Light::SetColor(const glm::vec3& color)
{
	m_Color = color;
}

void Dot::Light::SetStrength(const float& strength)
{
	m_Strength = strength;
}
