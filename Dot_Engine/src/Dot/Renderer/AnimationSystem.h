#pragma once
#include "Dot/ECS/ECSManager.h"
#include "Renderable/AnimatedMesh.h"


namespace Dot {
	class AnimationSystem : public System
	{
	public:
		void Update(float ts);
		virtual void Add(Entity entity);
		virtual void Remove(Entity entity);
		virtual void EntityUpdated(Entity entity);

	private:
		void readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
		glm::vec3 calcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::quat calcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::vec3 calcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

		unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	private:
		std::vector<Entity> m_Entities;
		AnimationComponent* m_Current;


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