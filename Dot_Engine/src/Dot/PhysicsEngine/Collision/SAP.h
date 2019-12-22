#pragma once
#include "Dot/PhysicsEngine/Components/Entity.h"

namespace Dot {
	
	class InteractionEngine
	{
	public:
		void Push(Ref<Entity> entity);

		void ProcessInteractions(float dt);
		void Remove(int index);

	private:
		bool Intersect(unsigned int firstIndex,unsigned int secondIndex);
	private:
		std::vector<Ref<Entity>> m_Entities;


	private:
		struct InteractionCompare
		{
			InteractionCompare(unsigned int axis)
				:m_Axis(axis)
			{}
			unsigned int m_Axis = 0;

			bool operator()(Ref<Entity> a, Ref<Entity> b)
			{
				return (a->m_AABB.GetMin()[m_Axis] < b->m_AABB.GetMin()[m_Axis]);
			}
		};
		InteractionCompare m_Cmp;
	};
}