#pragma once
#include "Terrain.h"
#include "Dot/Renderer/Shader/Shader.h"
#include "Dot/Core/Editor.h"
#include "Dot/Utils/MousePicker.h"

namespace Dot {
	
	class TerrainEditor : public Editor
	{
	public:
		class TerrainBrush
		{
			friend class TerrainEditor;
		public:
			TerrainBrush();
			~TerrainBrush();

			void ApplyBrush(TerrainEditor* editor);
			void SetPosition(const glm::vec3& position) { m_Position = position; }
			void SetRadius(unsigned int radius) { m_Radius = radius; }
			void SetSmooth(float smooth) { m_Smooth = smooth; }

		private:
			float smoothPoint(int x, int z,const Ref<Terrain>& terrain);
		private:
			float m_Radius;
			float m_Strength;
			float m_Smooth;

			glm::vec3 m_Position;

			Ref<Shader> m_ComputeBrush;
		};
	public:
		virtual void OnCreate(Entity entity) override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnDestroy() override;

		void UpdateBrush(const Camera& camera);
		Ref<Terrain>& GetTerrain() { return m_Terrain; }
		// Function to update Terrain
	private:
		void LoadHeightMap(const std::string& filePath);
		void DiamondSquare();
		void HeightsValueNoise();

		void ApplyNormals();
		void ApplyHeights();

	private:
		// Helper functions 
		glm::vec3 generateNormal(int x, int z) const;
		float getNoise(int x, int z);
		float getSmoothNoise(int x, int z);
		float getInterpolatedNoise(float x, float z);
		float interpolateCosine(float a, float b, float blend);
		float generateHeight(int x, int z);
		float getHeight(int x, int z) const;


		void diamondSquare(int size);
		void squareStep(int x, int z, int reach);
		void diamondStep(int x, int z, int reach);
	private:
		Ref<Terrain> m_Terrain;
		Ref<Material> m_Material;
		Ref<Shader>m_Shader;
		Transform *m_Transform;

		TerrainBrush m_Brush;
		MousePicker m_Picker;
	private:
		float m_Amplitude = 5.0f;
		float m_Roughness = 0.2f;
		float m_Height = 3.0f;

		int m_Octaves = 4;
		int m_Seed = 0;

	};
	
}