#pragma once
#include "Dot/Renderer/Renderable.h"

#include <glm/glm.hpp>

namespace Dot {


	class Water : public Renderable
	{
	public:
		Water(const glm::vec3& position,const glm::vec3& color,const glm::vec2& size, const float vertnum);

		void Update(float dt);
		
		void SetWaveLen(float len) { m_WaveLength = len; }
		void SetWaveAmpl(float ampl) { m_WaveAmplitude = ampl; }
		void SetColor(const glm::vec3& color) { m_Color = color; }

		virtual void Render(const Ref<Shader>& shader) override;

		const float &GetWaveLen() const { return m_WaveLength; }
		const float &GetWaveAmpl() const { return m_WaveAmplitude; }	
		const float& GetHeight() const { return m_Height; }
		const Ref<ArrayBuffer>& GetVAO() const { return m_VAO; }
		
	private:
		Ref<ArrayBuffer>m_VAO;
		float m_Height;

	private:
		glm::vec3 m_Color;
		float m_TimePass = 0.0f;
		float m_WaveLength = 30.0f;
		float m_WaveAmplitude = 2.0f;
	
	};
}