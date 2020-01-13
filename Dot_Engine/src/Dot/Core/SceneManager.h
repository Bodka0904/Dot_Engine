#pragma once
#include "Scene.h"


namespace Dot {
	class SceneManager
	{
	public:
		SceneManager();

		void Update(float dt);
		void Draw();

		unsigned int Add(Ref<Scene> scene);
		void SwitchScene(unsigned int id);
		void RemoveScene(unsigned int id);

		static Ref<SceneManager> Get() { return m_Manager; }
	private:
		static Ref<SceneManager> m_Manager;

		std::unordered_map<unsigned int, std::shared_ptr<Scene>> m_Scenes;
		Ref<Scene> m_CurrentScene;

		unsigned int m_InsertedSceneID;
	};
}