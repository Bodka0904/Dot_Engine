#include "Player.h"

Player::Player(const std::string& model, const std::string& texture)
{
	m_Material = std::make_shared<Dot::Material>(Dot::AssetManager::Get()->GetShader("AnimShader"));
	m_Material->Set("u_Texture",Dot::AssetManager::Get()->GetTexture(texture));
	m_Material->Set(Dot::RenderMode::OPAQUE);
	m_Material->Set("u_Reflectivity", 1.0f);
	m_Player = Dot::ECSManager::Get()->CreateEntity();
	Dot::MaterialStack::Get()->RegisterMaterial("Player", m_Material);

	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::Transform{});
	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::RigidBody{ glm::vec3{0, 0, 0} });
	

	Dot::Ref<Dot::Animation> animationWalk = std::make_shared<Dot::Animation>("res/Animations/man.fbx");
	Dot::Ref<Dot::Animation> animationAttack = std::make_shared<Dot::Animation>("res/Animations/attack@man.fbx");
	Dot::Ref<Dot::SkinnedMesh> mesh = std::make_shared<Dot::SkinnedMesh>("res/Animations/man.fbx");

	mesh->SetAnimation(animationWalk);
	mesh->material = m_Material;

	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::RenderComponent{
			mesh,
			Dot::MaterialStack::Get()->GetMaterialID("Player")
		});
	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::AnimationComponent{
		});

	auto animationComponent = &Dot::ECSManager::Get()->GetComponent<Dot::AnimationComponent>(m_Player);
	animationComponent->animation["walk"] = animationWalk;
	animationComponent->animation["attack"] = animationAttack;

	m_Transform = &Dot::ECSManager::Get()->GetComponent<Dot::Transform>(m_Player);
	m_RigidBody = &Dot::ECSManager::Get()->GetComponent<Dot::RigidBody>(m_Player);
	m_Animation = &Dot::ECSManager::Get()->GetComponent<Dot::AnimationComponent>(m_Player);

	Dot::ECSManager::Get()->SaveEntity(m_Player);
}

Player::~Player()
{
	Dot::ECSManager::Get()->DestroyEntity(m_Player);
}

void Player::Update(float speed, float rot, float dt, Dot::Terrain& terrain)
{
	if (Dot::Input::IsKeyPressed(Dot::D_KEY_UP))
	{
		m_RigidBody->velocity.z = speed * cos(m_Transform->rot.y);
		m_RigidBody->velocity.x = speed * sin(m_Transform->rot.y);
		//m_Transform->pos.y = terrain.GetHeight(m_Transform->pos);
		m_Animation->isAnimating = true;
	}
	else
	{
		m_RigidBody->velocity.z = 0;
		m_RigidBody->velocity.x = 0;
		m_Animation->isAnimating = false;
	}

	if (Dot::Input::IsKeyPressed(Dot::D_KEY_LEFT))
	{
		m_Transform->rot.y += rot * dt;
	}
	if (Dot::Input::IsKeyPressed(Dot::D_KEY_RIGHT))
	{
		m_Transform->rot.y -= rot * dt;
	}
}