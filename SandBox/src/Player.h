#pragma once
#include <Dot.h>
#include <math.h>

class Player
{
public:
	Player(const std::string& model, const std::string& texture);
	void Render();
	void Update(float speed, float rot, float dt, Dot::Light& light, Dot::Terrain& terrain);

	const glm::mat4& GetTransform() const { return m_Model; }
private:
	
	Dot::Ref<Dot::Texture2D> m_CowBoyTex;
	Dot::Ref<Dot::AnimatedMesh> m_Cowboy;

	Dot::Ref<Dot::Shader> m_Shader;

	glm::mat4 m_Model;

	
	Dot::RigidBody* m_RigidBody;
	Dot::Transform* m_Transform;
	Dot::Entity m_Player;
};