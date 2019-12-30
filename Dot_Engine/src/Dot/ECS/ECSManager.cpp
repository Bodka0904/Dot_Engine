#include "stdafx.h"
#include "ECSManager.h"


namespace Dot {
	Scope<ECSManager> ECSManager::m_This = std::make_unique<ECSManager>();
}