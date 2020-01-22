#pragma once
#include "Dot/Renderer/Renderable/Renderable.h"
#include "Dot/Renderer/Renderable/StaticMesh.h"
#include <glm/glm.hpp>

namespace Dot {


	class Water
	{
	public:
		Water(const glm::vec3& position,const glm::vec3& color,const glm::vec2& size, const float vertnum);

		void Update(float dt);
		
		void SetWaveLen(float len) { m_WaveLength = len; }
		void SetWaveAmpl(float ampl) { m_WaveAmplitude = ampl; }
		void SetColor(const glm::vec3& color) { m_Color = color; }

		void Render(const Ref<Shader>& shader);

		const float &GetWaveLen() const { return m_WaveLength; }
		const float &GetWaveAmpl() const { return m_WaveAmplitude; }	
		const float& GetHeight() const { return m_Height; }	
	private:
		Ref<Mesh>m_Mesh;
		float m_Height;

	private:
		glm::vec3 m_Color;
		float m_TimePass = 0.0f;
		float m_WaveLength = 30.0f;
		float m_WaveAmplitude = 2.0f;
	
	};
}