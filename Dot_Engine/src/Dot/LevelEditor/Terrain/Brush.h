#pragma once
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/LevelEditor/Terrain/Terrain.h"
#include "Dot/LevelEditor/Terrain/BrushInstance.h"

#include <glm/glm.hpp>

namespace Dot {
	class Brush
	{
	public:
		Brush(unsigned int intensity, float radius, float density);
		~Brush();

		void Place(const glm::vec3& position,Ref<BrushInstance>instance, Ref<Terrain> terrain);
		
		void SetIntensity(unsigned int intensity);
		void SetRadius(unsigned int radius);
		void SetDensity(double density);

		const unsigned int GetIntensity() const { return m_Intensity; }
		const unsigned int GetRadius() const { return m_Radius; }
	private:
		glm::vec2 CalcNextPosition(const glm::vec3& previousPosition, const glm::vec3& emitPlace);
	private:
		unsigned int m_Intensity;
		float		 m_Radius;
		double		 m_Density;

		Transform m_Transform;
	};
}
