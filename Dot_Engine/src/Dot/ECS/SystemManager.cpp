#include "stdafx.h"
#include "SystemManager.h"

namespace Dot {
	void System::Remove(Entity entity)
	{
		std::sort(m_Entities.begin(), m_Entities.end());
		
		int position = binarySearch(0, m_Entities.size()-1, entity);
		if (position != -1 && !m_Entities.empty())
		{
			m_Entities[position] = m_Entities[m_Entities.size() - 1];
			m_Entities.erase(m_Entities.end()-1);
		}
	}
	
	int System::binarySearch(int start, int end, Entity entity)
	{
		if (end >= start)
		{
			int mid = start + (end - start) / 2;
			if (m_Entities[mid] == entity) 
				return mid;
			
			if (m_Entities[mid] > entity)
				return binarySearch(start, mid - 1, entity);


			return binarySearch(mid + 1, end, entity);
		}
		return -1;
	}
}