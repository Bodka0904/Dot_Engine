#include "stdafx.h"
#include "Layer.h"

namespace Dot {
	Layer::Layer()
	{
	}


	Layer::~Layer()
	{
	}
	void Layer::Add(Entity entity)
	{
		m_Entities.push_back(entity);
	}
	void Layer::Remove(Entity entity)
	{
		std::sort(m_Entities.begin(), m_Entities.end(), m_Cmp);
		// get the range in 2*log2(N), N=vec.size()
		auto bounds = std::equal_range(m_Entities.begin(), m_Entities.end(), entity);
		// calculate the index of the first to be deleted O(1)
		auto last = m_Entities.end() - std::distance(bounds.first, bounds.second);
		// swap the 2 ranges O(equals) , equal = std::distance(bounds.first, bounds.last)
		std::swap_ranges(bounds.first, bounds.second, last);
		// erase the victims O(equals)
		m_Entities.erase(last, m_Entities.end());
	}
}