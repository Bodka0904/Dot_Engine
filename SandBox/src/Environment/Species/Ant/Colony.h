#pragma once
#define DEFAULT_COLONY_SIZE 10
#define MAX_COLONY_SIZE 50

#include <Dot.h>

#include "Ant.h"

class Colony
{
public:
	Colony(const std::string& fileName)
	{
		std::vector<glm::mat4> transformations;
		transformations.resize(MAX_COLONY_SIZE);

		for (int i = 0; i < DEFAULT_COLONY_SIZE; ++i)
		{
			m_Ants.push_back(Ant(i,glm::vec3(20,0,20)));
			transformations[i] = m_Ants[i].GetTransformation();
		}
		for (int i = DEFAULT_COLONY_SIZE; i < MAX_COLONY_SIZE; ++i)
		{
			transformations[i] = glm::mat4(1);
		}

		m_Model = std::make_shared<Dot::InstancedMesh>(fileName, transformations);
	}
	~Colony() {};

	void Update()
	{
		std::vector<glm::mat4> transforms;
		transforms.resize(m_Ants.size());
		for (int i = 0; i < m_Ants.size(); ++i)
		{
			m_Ants[i].Move(glm::vec3(0.005,0,0.005));
			transforms[i] = m_Ants[i].GetTransformation();
		}
		m_Model->Update(transforms,m_Ants.size());
	
	}

	void IncreaseAnt(int num,const glm::vec3& pos)
	{
		for (int i = 0; i < num; ++i)
		{
			if (m_Ants.size() + 1 <= MAX_COLONY_SIZE)
			{
				m_Ants.push_back(Ant(m_Ants.size() + i, pos));
			}
		}
	}
	void DecreaseAnt(int index)
	{
		if (m_Ants.size() > 0)
		{
			m_Ants.erase(m_Ants.begin() + index);
		}
	}


	const int& GetSize() const { return m_Ants.size(); }
	const Dot::Ref<Dot::InstancedMesh>& GetModel() const { return m_Model; }
private:

	std::vector<Ant> m_Ants;
	Dot::Ref<Dot::InstancedMesh> m_Model;

};