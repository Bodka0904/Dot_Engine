#include "Player.h"

Player::Player(const std::string& model, const std::string& texture)
{
	m_Model = Dot::AssetManager::Get()->GetAnimMesh(model);
	m_Model->AnimateBones(0);
	
	m_Material = std::make_shared<Dot::Material>(Dot::AssetManager::Get()->GetShader("AnimShader"));
	m_Material->Set(Dot::AssetManager::Get()->GetTexture(texture));
	m_Material->Set(Dot::RenderMode::OPAQUE);
	m_Player = Dot::ECSManager::Get()->CreateEntity();

	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::Transform{ glm::vec3(0, 0, 0),glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)});
	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::RigidBody{ glm::vec3{0, 0, 0} });
	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::RenderComponent{ 
		Dot::AssetManager::Get()->GetAnimMesh(model), 
		m_Material
		});
	
	m_RigidBody = &Dot::ECSManager::Get()->GetComponent<Dot::RigidBody>(m_Player);
	m_Transform = &Dot::ECSManager::Get()->GetComponent<Dot::Transform>(m_Player);
}

Player::~Player()
{
	Dot::ECSManager::Get()->DestroyEntity(m_Player);
}

void Player::Update(float speed, float rot, float dt, Dot::Terrain& terrain)
{
	if (Dot::Input::IsKeyPressed(D_KEY_UP))
	{
		m_RigidBody->velocity.z = speed * cos(m_Transform->rot.y);
		m_RigidBody->velocity.x = speed * sin(m_Transform->rot.y);
		m_Transform->pos.y = terrain.GetHeight(m_Transform->pos);

		m_Model->AnimateBones(dt);
	}
	else
	{
		m_Model->SetToDefaultPosition();
		m_RigidBody->velocity.z = 0;
		m_RigidBody->velocity.x = 0;
	}

	if (Dot::Input::IsKeyPressed(D_KEY_LEFT))
	{
		m_Transform->rot.y += rot * dt;	
	}
	if (Dot::Input::IsKeyPressed(D_KEY_RIGHT))
	{
		m_Transform->rot.y -= rot * dt;
	}	
}
