#include "stdafx.h"
#include "Brush.h"
#include "Dot/Utils/Random/RandomGenerator.h"

namespace Dot {
	Brush::Brush(unsigned int intensity, float radius, float density)
		:
		m_Intensity(intensity),
		m_Radius(radius),
		m_Density(density)
	{
		m_Transform.m_Rot = glm::vec3(0, 0, 0);
		m_Transform.m_Scale = glm::vec3(1, 1, 1);
	}
	Brush::~Brush()
	{
	}
	void Brush::Place(const glm::vec3& position, Ref<BrushInstance>instance, Ref<Terrain> terrain)
	{
		std::vector<glm::mat4> transforms;
		transforms.resize(m_Intensity);

		m_Transform.m_Pos = position;
		for (int i = 0; i < m_Intensity; ++i)
		{
			glm::vec2 nextPos = CalcNextPosition(m_Transform.m_Pos, position);
			m_Transform.m_Pos = glm::vec3(nextPos.x, terrain->GetHeight(glm::vec3(nextPos.x, 0, nextPos.y)), nextPos.y);
			m_Transform.m_Rot.y = RandomGenerator::Random(0, 90);
			transforms[i] = m_Transform.GetModel();
		}
		instance->Update(transforms);
	}
	void Brush::SetIntensity(unsigned int intensity)
	{
		m_Intensity = intensity;
	}
	void Brush::SetRadius(unsigned int radius)
	{
		m_Radius = radius;
	}
	void Brush::SetDensity(double density)
	{
		m_Density = density;
	}
	glm::vec2 Brush::CalcNextPosition(const glm::vec3& previousPosition, const glm::vec3& emitPlace)
	{
		double x;
		double z;
		if (previousPosition.x + m_Density > emitPlace.x + m_Radius)
		{
			x = RandomGenerator::Random(previousPosition.x - m_Density, previousPosition.x);
		}
		else if (previousPosition.x - m_Density < emitPlace.x - m_Radius)
		{
			x = RandomGenerator::Random(previousPosition.x, previousPosition.x + m_Density);
		}
		else
		{
			x = RandomGenerator::Random(previousPosition.x - m_Density, previousPosition.x + m_Density);
		}

		if (previousPosition.z + m_Density > emitPlace.z + m_Radius)
		{
			z = RandomGenerator::Random(previousPosition.z - m_Density, previousPosition.z);
		}
		else if (previousPosition.z - m_Density < emitPlace.z - m_Radius)
		{
			z = RandomGenerator::Random(previousPosition.z, previousPosition.z + m_Density);
		}
		else
		{
			z = RandomGenerator::Random(previousPosition.z - m_Density, previousPosition.z + m_Density);
		}
		return glm::vec2(x, z);
	}
}