#include "Player.h"

Player::Player(const std::string& model, const std::string& texture)
{
	m_Cowboy = Dot::AssetManager::Get()->GetAnimMesh(model);
	m_Cowboy->AnimateBones(0);
	m_CowBoyTex = Dot::AssetManager::Get()->GetTexture(texture);

	m_Shader = Dot::Shader::Create("AnimationShader", "res/shaders/Dot/AnimShader.glsl");
	m_Shader->AddUniform("u_ModelMatrix");
	m_Shader->AddUniform("u_LightPosition");
	m_Shader->AddUniform("u_LightColor");
	m_Shader->AddUniform("u_LightStrength");

	for (size_t i = 0; i < m_Cowboy->GetBoneTransformations().size(); i++)
	{
		std::string uniformName = std::string("u_gBones[") + std::to_string(i) + std::string("]");
		m_Shader->AddUniform(uniformName);
	}
		
	m_Player = Dot::ECSManager::Get()->CreateEntity();
	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::Transform{ glm::vec3(0, 0, 0),glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)});
	Dot::ECSManager::Get()->AddComponent(m_Player, Dot::RigidBody{ glm::vec3{0, 0, 0} });

	m_RigidBody = &Dot::ECSManager::Get()->GetComponent<Dot::RigidBody>(m_Player);
	m_Transform = &Dot::ECSManager::Get()->GetComponent<Dot::Transform>(m_Player);
	m_Model = m_Transform->GetModel();
}

void Player::Render()
{
	m_CowBoyTex->Bind(0);
	m_Cowboy->Render();
}

void Player::Update(float speed, float rot, float dt, Dot::Light& light, Dot::Terrain& terrain)
{
	if (Dot::Input::IsKeyPressed(D_KEY_UP))
	{
		m_RigidBody->m_Velocity.z = speed * cos(m_Transform->m_Rot.y);
		m_RigidBody->m_Velocity.x = speed * sin(m_Transform->m_Rot.y);
		m_Transform->m_Pos.y = terrain.GetHeight(m_Transform->m_Pos);

		m_Cowboy->AnimateBones(dt);
		m_Model = m_Transform->GetModel();
	}
	else
	{
		m_Cowboy->SetToDefaultPosition();
		m_Model = m_Transform->GetModel();
		m_RigidBody->m_Velocity.z = 0;
		m_RigidBody->m_Velocity.x = 0;
	}

	if (Dot::Input::IsKeyPressed(D_KEY_LEFT))
	{
		m_Transform->m_Rot.y += rot * dt;
		m_Model = m_Transform->GetModel();
		
	}
	if (Dot::Input::IsKeyPressed(D_KEY_RIGHT))
	{
		m_Transform->m_Rot.y -= rot * dt;
		m_Model = m_Transform->GetModel();
	}

	m_Shader->Bind();

	for (size_t i = 0; i < m_Cowboy->GetBoneTransformations().size(); i++)
	{
		std::string uniformName = std::string("u_gBones[") + std::to_string(i) + std::string("]");
		m_Shader->UploadUniformMat4(uniformName, m_Cowboy->GetBoneTransformations()[i]);
	}
	m_Shader->UploadUniformMat4("u_ModelMatrix", m_Model);
	m_Shader->UploadUniformFloat3("u_LightPosition", light.GetPosition());
	m_Shader->UploadUniformFloat3("u_LightColor", light.GetColor());
	m_Shader->UploadUniformFloat("u_LightStrength", light.GetStrength());
}
