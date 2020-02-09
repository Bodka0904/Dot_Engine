#include "stdafx.h"
#include "SceneManager.h"

namespace Dot {
	Ref<SceneManager> SceneManager::m_Manager = std::make_shared<SceneManager>();
	
	SceneManager::SceneManager()
		:m_InsertedSceneID(0)
	{
	}
	void SceneManager::Update(float dt)
	{
		m_CurrentScene->OnUpdate(dt);
	}
	
	unsigned int SceneManager::Add(Ref<Scene> scene)
	{
		scene->OnActivate();
		m_Scenes[m_InsertedSceneID] = scene;
		m_InsertedSceneID++;

		return m_InsertedSceneID - 1;
	}
	void SceneManager::SwitchScene(unsigned int id)
	{
		auto it = m_Scenes.find(id);
		if (it != m_Scenes.end())
		{
			if (m_CurrentScene)
			{
				m_CurrentScene->OnDeactivate();
			}
			m_CurrentScene = it->second;
			m_CurrentScene->OnActivate();
		}
	}
	void SceneManager::RemoveScene(unsigned int id)
	{
		auto it = m_Scenes.find(id);
		if (it != m_Scenes.end())
		{
			if (m_CurrentScene == it->second)
			{
				m_CurrentScene = nullptr;
			}

			it->second->OnDestroy();
			m_Scenes.erase(it);
		}
	}
}