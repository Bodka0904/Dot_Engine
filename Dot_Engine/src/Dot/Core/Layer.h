#pragma once
#include "Events/Event.h"
#include "Dot/Utils/Time/Timestep.h"
#include "Dot/ECS/ECSManager.h"

namespace Dot {
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void OnAttach() {} //Init
		virtual void OnDetach() {} //Destroy
		virtual void OnUpdate(Timestep ts) {} //Update
		virtual void OnEvent(Event& event) {}

		virtual void Add(Entity entity);
		virtual void Remove(Entity entity);

	private:
		std::vector<Entity> m_Entities;

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