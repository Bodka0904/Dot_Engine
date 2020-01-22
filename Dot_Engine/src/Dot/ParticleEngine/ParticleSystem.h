#pragma once
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/Renderer/Buffers/Buffer.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/ECS/ECSManager.h"

namespace Dot {
#define MAX_PARTICLES 10000

	class ParticleSystem : public System
	{
	public:
		struct ParticleVertexData
		{
			// position , transparency
			glm::vec4 position;
		};

		struct ParticleData
		{
			// velocity, timeAlive
			glm::vec4 velocity;
			float lifeSpan;
			float gravity;
			float unUsed;
			float unUsed2;
		};
		struct ParticleEffect
		{
			Entity entity;
			std::pair<int, Ref<ShaderStorageBuffer>> BufferVertex;
			std::pair<int, Ref<ShaderStorageBuffer>> BufferData;
		};
	public:
		ParticleSystem();
		virtual void Update(const Camera& camera,float ts);
		virtual void Add(Entity entity) override;
		virtual void Remove(Entity entity) override;
		virtual void EntityUpdated(Entity entity) override;
	
	private:
		int binarySearch(int start, int end,const std::vector<ParticleEffect>&container, Entity entity);

	private:
		std::unordered_map<Ref<Shader>, std::vector<ParticleEffect>> m_Effect;
		

		struct Compare
		{
			bool operator()(ParticleEffect a, ParticleEffect b)
			{
				return (a.entity < b.entity);
			}
		};
		Compare m_Cmp;
	};
}