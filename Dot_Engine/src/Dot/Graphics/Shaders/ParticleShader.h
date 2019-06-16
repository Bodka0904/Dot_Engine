#pragma once
#include "Shader.h"
#include "Dot/Graphics/Particle/ParticleEffect.h"
#include "Dot/Graphics/Transform.h"
#include "Dot/Graphics/Camera.h"

class ParticleShader : Shader 
{
public:
	ParticleShader();
	virtual ~ParticleShader();

	virtual void Init(const std::string& filename) override;
	virtual void SetAttribs() override;
	virtual void SetUniforms() override;
	
	virtual void Update(const Transform& transform, Camera& camera);

	virtual void LinkShader();
	virtual void Bind();
	virtual void UnBind();
private:
	static const unsigned int NUM_SHADER = 2;


	GLuint m_program;
	GLuint m_shaders[NUM_SHADER];
	GLuint m_uniforms[MAX_PARTICLES];
};

