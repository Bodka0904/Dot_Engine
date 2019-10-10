#pragma once
#include <Dot.h>
#include <math.h>

class Player
{
public:
	Player(const std::string& model,const std::string& texture)
	{
		m_Cowboy = std::make_shared<Dot::AnimatedMesh>(model);
		m_Cowboy->AnimateBones(0);
		m_CowBoyTex = std::make_shared<Dot::Texture>(texture);

		m_Shader = std::make_shared<Dot::Shader>("AnimationShader","res/shaders/Dot/AnimShader.glsl");
		m_Shader->AddUniform("u_ModelMatrix");
		m_Shader->AddUniform("u_LightPosition");
		m_Shader->AddUniform("u_LightColor");
		m_Shader->AddUniform("u_LightStrength");

		for (size_t i = 0; i < m_Cowboy->GetBoneTransformations().size(); i++)
		{
			std::string uniformName = std::string("u_gBones[") + std::to_string(i) + std::string("]");
			m_Shader->AddUniform(uniformName);
		}

	}

	void Render()
	{
		m_CowBoyTex->Bind(0);
		m_Cowboy->Render();
	}
	void Update(float speed,float rot,float dt,Dot::Light& light,Dot::Terrain& terrain)
	{
		if (Dot::Input::IsKeyPressed(D_KEY_UP))
		{
			m_Transform.GetPos().z += speed * cos(m_Transform.GetRot().y);
			m_Transform.GetPos().x += speed * sin(m_Transform.GetRot().y);
			m_Transform.GetPos().y = terrain.GetHeight(m_Transform.GetPos());
			m_Cowboy->AnimateBones(dt);
		}
		if (Dot::Input::IsKeyPressed(D_KEY_LEFT))
		{
			m_Transform.GetRot().y += rot;
		}
		if (Dot::Input::IsKeyPressed(D_KEY_RIGHT))
		{
			m_Transform.GetRot().y -= rot;
		}

		m_Transform.UpdateModel();
		
		m_Shader->Bind();
		
		for (size_t i = 0; i < m_Cowboy->GetBoneTransformations().size(); i++)
		{
			std::string uniformName = std::string("u_gBones[") + std::to_string(i) + std::string("]");
			m_Shader->UploadUniformMat4(uniformName, m_Cowboy->GetBoneTransformations()[i]);
		}
		m_Shader->UploadUniformMat4("u_ModelMatrix", m_Transform.GetModel());
		m_Shader->UploadUniformFloat3("u_LightPosition", light.GetPosition());
		m_Shader->UploadUniformFloat3("u_LightColor", light.GetColor());
		m_Shader->UploadUniformFloat("u_LightStrength", light.GetStrength());
	}


	const glm::mat4& GetTransform() const { return m_Transform.GetModel(); }

private:
	Dot::Ref<Dot::Texture> m_CowBoyTex;
	Dot::Ref<Dot::AnimatedMesh> m_Cowboy;

	Dot::Ref<Dot::Shader> m_Shader;
	Dot::Transform m_Transform;


};