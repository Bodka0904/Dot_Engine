#pragma once

#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/ECS/ECSManager.h"
#include "ParticleEffect.h"

namespace Dot {
	class ParticleSystem : public System
	{
	public:
		ParticleSystem();
		virtual void Update(const Camera& camera,float ts);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
	private:
		std::unordered_map<std::string, std::vector<Entity>> m_Entities;

	private:
		struct Compare
		{
			bool operator()(Entity a, Entity b)
			{
				return (a < b);
			}
		};
		Compare m_Cmp;
	};
}