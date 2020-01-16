#pragma once
#include "Dot/Renderer/Buffers/ArrayBuffer.h"
#include "Dot/PhysicsEngine/Components/ComponentData.h"
#include "Dot/Renderer/StaticMesh.h"
#include "Dot/ECS/ECSManager.h"
#include <glm/glm.hpp>

namespace Dot {

	class Terrain;
	class TerrainBrush
	{
	public:
		TerrainBrush(float radius, Ref<Terrain> terrain);
		~TerrainBrush();

		void ApplyBrush();

		void SetPosition(const glm::vec3& position) { m_Transform->pos = position; }
		void SetRadius(unsigned int radius) { m_Radius = radius; }
		void SetSmooth(float smooth) { m_Smooth = smooth; }

		const unsigned int GetRadius() const { return m_Radius; }
		const glm::vec3& GetPosition() const { return m_Transform->pos; }
	private:
		float smoothPoint(int x,int z);
	private:
		float m_Radius;
		float m_Strength;
		float m_Smooth;

		Entity m_Entity;
		Ref<Terrain> m_Terrain;

		Transform* m_Transform;
	};

	class TerrainEditor;
	class Terrain
	{
		friend class TerrainEditor;
		friend class TerrainBrush;
	public:
		Terrain(float size, int numvertex);
		Terrain(const std::string& heightMap,float size,float height);
		~Terrain();
	
		// Function to update Terrain 
		void DiamondSquare();
		void HeightsValueNoise();

		void ApplyNormals();
		void ApplyHeights();
	public:
		// Control of params for Terrain generation
		void SetSeed(int num) { m_Seed = num; }
		void SetAmplitude(float amplitude) { m_Amplitude = amplitude; }
		void SetOctaves(int octaves) { m_Octaves = octaves; }
		void SetRoughness(float roughness) { m_Roughness = roughness; }
		void SetHeight(float height) { m_Height = height; }

		// Getter functions
		const float& GetAmplitude() const { return m_Amplitude; }
		const int& GetOctaves() const { return m_Octaves; }
		const float& GetRoughness() const { return m_Roughness;}
		const float& GetSize() const { return m_Size; }
		
		const Ref<ArrayBuffer>& GetVAO() const { return m_VAO; }

		// Get height of surface in the position
		float GetHeight(const glm::vec3& position);
	
	private:
		// Helper functions 
		float getNoise(int x, int z);
		float getSmoothNoise(int x, int z);
		float getInterpolatedNoise(float x, float z);
		float interpolateCosine(float a, float b, float blend);
		float generateHeight(int x, int z);
		float getHeight(int x, int z) const;
	
		glm::vec3 generateNormal(int x, int z) const;
	
		void diamondSquare(int size);
		void squareStep(int x, int z, int reach);
		void diamondStep(int x, int z, int reach);
	private:
		float barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
	
	private:
		std::shared_ptr<ArrayBuffer> m_VAO;
		std::vector<glm::vec3> m_Vertices;
		std::vector<std::vector<float> > m_Heights;


	private:
		float m_Amplitude = 5.0f;
		float m_Roughness = 0.2f;
		float m_Height = 3.0f;
		
		int m_Octaves = 4;
		int m_Seed;
		int m_NumVertex;
		float m_Size;
			
	};



}

