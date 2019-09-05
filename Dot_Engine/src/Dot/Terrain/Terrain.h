#pragma once
#include "Dot/Graphics/Renderer/Buffers/ArrayBuffer.h"
#include <glm/glm.hpp>

namespace Dot {

	class Terrain
	{
	public:
		Terrain(float size, unsigned int numvertex);
		~Terrain();

		void ApplyHeightsValueNoise(int height);
		void ApplyNormals();

		void SetSeed(int num) { m_Seed = num; }
		void SetAmplitude(float amplitude) { m_Amplitude = amplitude; }
		void SetOctaves(int octaves) { m_Octaves = octaves; }
		void SetRoughness(float roughness) { m_Roughness = roughness; }
		void SetSize(float size) { m_Size = size; }

		const float& GetAmplitude() const { return m_Amplitude; }
		const int& GetOctaves() const { return m_Octaves; }
		const float& GetRoughness() const { return m_Roughness; }
		const float& GetSize() const { return m_Size; }
		const Ref<ArrayBuffer>& GetVao() const { return m_VAO; }

		float GetHeight(const glm::vec3& position);

	private:
		float getNoise(int x, int z);
		float getSmoothNoise(int x, int z);
		float getInterpolatedNoise(float x, float z);
		float interpolateCosine(float a, float b, float blend);
		float generateHeight(int x, int z);
		float getHeight(int x, int z) const;

		glm::vec3 generateNormal(int x, int z) const;

	private:
		float barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);

	private:
		std::shared_ptr<ArrayBuffer> m_VAO;
		std::vector<std::vector<float> > m_Heights;

	private:
		// TODO AFTER GENERATING SHOULD BE DELETED
		std::vector<float> vertices;
		std::vector<float> texcoords;
		std::vector<float> normals;


	private:
		float m_Amplitude = 5.0f;
		int m_Octaves = 4;
		float m_Roughness = 0.2f;
		int m_Height = 0;
		int m_Seed;
		float m_Size;
		unsigned int m_NumVertex;

		
	};
}

