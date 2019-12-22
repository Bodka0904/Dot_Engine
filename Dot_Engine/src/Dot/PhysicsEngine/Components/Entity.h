#pragma once
#include "Transform.h"
#include "AABB.h"

namespace Dot {
	struct Entity
	{
		Transform m_Transform;
		AABB      m_AABB;
	};
}