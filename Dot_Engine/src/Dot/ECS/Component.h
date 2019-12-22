#pragma once

#include "Entity.h"

namespace Dot {
	class IComponent
	{
	public:
		virtual ~IComponent() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class Component : public IComponent
	{
	public:	
		
		bool Contains(Entity entity)
		{
			return m_Lookup.find(entity) != m_Lookup.end();
		}

		void AddComponent(Entity entity, T component)
		{
			// Only one of this component type per entity is allowed!
			D_ASSERT(m_Lookup.find(entity) == m_Lookup.end(),"Entity already contains component");

			// Update the entity lookup table:
			m_Lookup[entity] = m_Num;
			m_Component[m_Num] = component;

			m_Num++;
		}
		void RemoveComponent(Entity entity)
		{
			D_ASSERT(m_Lookup.find(entity) != m_Lookup.end() , "Removing non-existent component");
			size_t indexRemovedEnity = m_Lookup[entity];
			// Put last component to the place of removed entity
			m_Component[indexRemovedEnity] = m_Component[m_Num - 1];
			// Set lookup for new place of last entity;
			m_Lookup[m_Num - 1] = indexRemovedEnity;

			// Erase from lookup deleted entity
			m_Lookup.erase(entity);

			m_Num--;
		}
		T* GetComponent(Entity entity)
		{
			D_ASSERT(m_Lookup.find(entity) != m_Lookup.end() , "Retrieving non-existent component.");
			
			return &m_Component[m_Lookup[entity]];
		}
		
		virtual void EntityDestroyed(Entity entity) override
		{
			if (m_Lookup.find(entity) != m_Lookup.end())
			{
				RemoveComponent(entity);
			}
		}
	
	private:
		std::array<T,MAX_ENTITIES> m_Component;
		std::unordered_map<Entity, size_t> m_Lookup;

		size_t m_Num = 0;
	};
}