#pragma once
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/ECS/ECSManager.h"

namespace Dot {
	class PhysicsSystem : public System
	{
	public:
		virtual void Update(float dt) override;

	private:
	};
}