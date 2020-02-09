#pragma once
#include "Scene.h"
#include "Dot/Renderer/Camera/Camera.h"
#include "Dot/Renderer/Light/Light.h"

namespace Dot {

	class Scene3D : public Scene
	{
	public:
		virtual void OnCreate() override;
		virtual void OnDestroy() override;
		virtual void OnActivate() override;
		virtual void OnDeactivate() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnRender() override;
		virtual void AddEntity(Entity entity) override;
		virtual void RemoveEntity(Entity entity) override;
	private:
		Ref<Camera> m_Camera;
	
	};
}