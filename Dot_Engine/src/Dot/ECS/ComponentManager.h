#pragma once
#include "Entity.h"
#include "Component.h"

#include <bitset>

namespace Dot {

	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();
			D_ASSERT(m_Components.find(typeName) == m_Components.end() , "Registering component type more than once.");
			
			m_ComponentTypes[typeName] = m_NextComponentType;

			Ref<Component<T>> comp = std::make_shared< Component<T> >();
			m_Components.insert({ typeName,comp });
			
			m_NextComponentType++;
		}
		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			const char* typeName = typeid(T).name();
			D_ASSERT(m_Components.find(typeName) != m_Components.end(),"Accessing not registered component.");
			GetComponentArray<T>()->AddComponent(entity, component);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			D_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(),"Component not registered before use.");

			// Return this component's type - used for creating signatures
			return m_ComponentTypes[typeName];
		}

		template<typename T>
		T* GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetComponent(entity);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}
		void EntityDestroyed(Entity entity)
		{
			for (auto const& it : m_Components)
			{
				it.second->EntityDestroyed(entity);
			}
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<IComponent> > m_Components;
		std::unordered_map<std::string, ComponentType > m_ComponentTypes;
		ComponentType m_NextComponentType = 0;

	private:
		template<typename T>
		std::shared_ptr<Component<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			D_ASSERT(m_Components.find(typeName) != m_Components.end() , "Component not registered before use.");

			return std::static_pointer_cast<Component<T>>(m_Components[typeName]);
		}
	};
	
}