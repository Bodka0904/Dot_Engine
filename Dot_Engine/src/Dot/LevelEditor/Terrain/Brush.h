#pragma once
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/LevelEditor/Terrain/Terrain.h"
#include "Dot/LevelEditor/Terrain/BrushInstance.h"
#include "Dot/Renderer/Renderable/StaticMesh.h"
#include "Dot/ECS/ECSManager.h"
#include <glm/glm.hpp>

namespace Dot {
	class Brush
	{
	public:
		Brush(unsigned int intensity, float radius, float density,Ref<Terrain> terrain);
		~Brush();

		void Place(const Ref<BrushInstance>& instance);
		

		void SetPosition(const glm::vec3& position);
		void SetIntensity(unsigned int intensity);
		void SetRadius(unsigned int radius);
		void SetDensity(double density);

		const unsigned int GetIntensity() const { return m_Intensity; }
		const unsigned int GetRadius() const { return m_Radius; }
		const glm::vec3& GetPosition() const { return m_Transform->pos; }
	private:

		unsigned int m_Intensity;
		float		 m_Radius;
		float		 m_Density;
	
		Entity m_Entity;
		Ref<Terrain> m_Terrain;

		Transform *m_Transform;
	};
}
