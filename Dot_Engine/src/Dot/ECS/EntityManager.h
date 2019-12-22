#pragma once
#include "Entity.h"

#include <bitset>
#include <queue>
#include <array>


namespace Dot {

	class EntityManager
	{
	public:
		EntityManager();
		
		Entity CreateEntity();
		Signature GetSignature(Entity entity);
		void DestroyEntity(Entity entity);
		void SetSignature(Entity entity, Signature signature);

	private:
		unsigned int m_EntitiesInUse;

		std::queue<Entity> m_AvailableEntities{};

		// Every entity has Signature representing components that it contains
		// System with same signatures will use entity
		std::array<Signature, MAX_ENTITIES> m_Signatures{};
	};
}