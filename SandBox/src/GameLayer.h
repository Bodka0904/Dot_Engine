#pragma once
#include "Player.h"
#include <Dot.h>


class GameLayer : public Dot::Layer
{
public:
	GameLayer();
	virtual void OnAttach() override;
	virtual void OnUpdate(Dot::Timestep ts) override;
	virtual void OnEvent(Dot::Event& event) override;
	virtual void OnDetach() override;
private:
	Dot::Ref<Dot::Shader> m_StaticShader;
	Dot::Ref<Dot::Shader> m_SkyShader;
	Dot::Ref<Dot::Shader> m_InstanceShader;
	Dot::Ref<Dot::Shader> m_GrassShader;
	Dot::Ref<Dot::Shader> m_ParticleComputeShader;
	Dot::Ref<Dot::Shader> m_ParticleShader;
	Dot::Ref<Dot::Shader> m_WaterShader;
	Dot::Ref<Dot::Shader> m_LightShader;

	Dot::Ref<Dot::Texture2D> m_TreeTexture;
	Dot::Ref<Dot::Texture2D> m_TerrTexture;
	Dot::Ref<Dot::Texture2D> m_ParticleTexture;
	Dot::Ref<Dot::Texture2D> m_GrassTexure;


	Dot::Ref<Dot::InstancedMesh> m_Tree;
	Dot::Ref<Player> m_Player;


	Dot::Ref<Dot::Terrain> m_Terrain;

	Dot::Ref<Dot::CameraController> m_CamController;
	

	Dot::Ref<Dot::MousePicker> m_Picker;
	Dot::Ref<Dot::BrushInstance> m_Grass;
	Dot::Ref<Dot::Brush>m_Brush;
	Dot::Ref<Dot::Light> m_Light;

	Dot::Ref<Dot::CollisionSystem> m_CollisionSystem;
	Dot::Ref<Dot::PhysicsSystem> m_PhysicsSystem;
	Dot::Ref<Dot::RenderSystem> m_RenderSystem;
	Dot::Ref<Dot::ParticleSystem> m_ParticleSystem;
	Dot::Ref<Dot::AnimationSystem> m_AnimatioSystem;
	//Dot::Ref<Dot::ParticleEditor> m_ParticleEditor;
	//Dot::Ref<Dot::RenderableEditor>m_RenderableEditor;
	Dot::Ref<Dot::Material> m_Material;
	Dot::Ref<Dot::Framebuffer> m_FrameBuffer;


	Dot::Ref<Dot::Shader> m_WaterTileShader;
	Dot::Ref<Dot::WaterTile> m_WaterTile;
	Dot::Ref<Dot::Texture2D> m_DuvMap;
	Dot::Ref<Dot::Texture2D> m_NormalMap;

	Dot::Ref<Dot::LevelEditor> m_LevelEditor;
	Dot::Ref<Dot::Material> m_SkyboxMaterial;
private:
	Dot::Transform m_Transform;

private:
	bool m_Swap = false;
	float m_TimePassed = 0.0f;
	float m_ParticleTime = 0.0f;


	Dot::Entity m_TerrainEntity;
	Dot::Entity m_SkyboxEntity;
};





