#pragma once
#include <Dot.h>
#include <math.h>

class Player
{
public:
	Player(const std::string& model, const std::string& texture);
	~Player();
	void Update(float speed, float rot, float dt, Dot::Terrain& terrain);

private:
	Dot::Ref<Dot::Material> m_Material;
	Dot::AnimationComponent* m_Animation;
	Dot::RigidBody* m_RigidBody;
	Dot::Transform* m_Transform;
	Dot::Entity m_Player;
};