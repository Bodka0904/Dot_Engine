#pragma once
#include "Types.h"

#include <set>
#include <bitset>

namespace Dot {	

	
	class System
	{
	public:
		System() {};
		virtual void Add(Entity entity) = 0;
		virtual void Remove(Entity entity) = 0;
		virtual void EntityUpdated(Entity entity) {};
	
	};


	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();
			D_ASSERT(m_Systems.find(typeName) == m_Systems.end() , "System already registered!");

			auto system = std::make_shared<T>();
			m_Systems[typeName] = system;

			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			D_ASSERT(m_Systems.find(typeName) != m_Systems.end(), "System is not registered!");

			m_Signatures[typeName] = signature;
		}
		template<typename T>
		Ref<System> GetSystem()
		{
			const char* typeName = typeid(T).name();
			D_ASSERT(m_Systems.find(typeName) != m_Systems.end(), "System is not registered!");
			return m_Systems[typeName];
		}
		void EntityDestroyed(Entity entity, Signature entitySignature)
		{
			// Erase a destroyed entity from all system lists
			for (auto const& it : m_Systems)
			{
				auto const& type = it.first;
				auto const& system = it.second;
				auto const& systemSignature = m_Signatures[type];

				if ((entitySignature & systemSignature) == systemSignature)
				{
					it.second->Remove(entity);
				}
			}
		}

		void AddEntity(Entity entity, Signature entitySignature)
		{
			for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->Add(entity);
				}
			}
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->Add(entity);
				}
				// Entity signature does not match system signature - erase from set
				else
				{
					system->Remove(entity);
				}
			}
		}
		

	private:
		std::unordered_map<const char*, Signature> m_Signatures;
		std::unordered_map<const char*, std::shared_ptr<System>> m_Systems;
	};
}