#include "stdafx.h"
#include "TerrainEditor.h"
#include "Dot/Graphics/Renderer/Renderer.h"
#include <random>
#include <GL/glew.h>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Dot {

	TerrainEditor::TerrainEditor()
	{
	}

	TerrainEditor::~TerrainEditor()
	{
	}

	void TerrainEditor::GenFlatTerrain(float size, unsigned int numvertex)
	{
		m_data.reset(new TerrainData);
		m_data->size = size;
		m_data->numvertex = numvertex;

		std::shared_ptr<VertexBuffer> vbo;
		std::shared_ptr<IndexBuffer> ibo;

		std::vector<unsigned int> indices;

		m_vertices.resize(m_data->numvertex * m_data->numvertex);
		indices.resize(6 * (m_data->numvertex - 1) * (m_data->numvertex - 1));
		int count = m_data->numvertex * m_data->numvertex;

		for (int i = 0; i < m_data->numvertex; i++)
		{
			for (int j = 0; j < m_data->numvertex; j++)
			{
				m_vertices[j + (i * m_data->numvertex)] =
					VertexTexture(
						glm::vec3((float)j / ((float)m_data->numvertex - 1) * m_data->size, 0, (float)i / ((float)m_data->numvertex - 1) * m_data->size),
						glm::vec3(0, 0, 0), 
						glm::vec2((float)j / ((float)m_data->numvertex - 1), (float)i / ((float)m_data->numvertex - 1)));
			}
		}
		int pointer = 0;
		for (int gz = 0; gz < m_data->numvertex - 1; gz++) {
			for (int gx = 0; gx < m_data->numvertex - 1; gx++) {
				int topLeft = (gz * m_data->numvertex) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * m_data->numvertex) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}

		BufferLayout layout = {
				{0, Dot::ShaderDataType::Float3, "position" },
				{1, Dot::ShaderDataType::Float3, "normal" },
				{2, Dot::ShaderDataType::Float2, "texCoord" },

		};

		m_VAO.reset(new ArrayBuffer());

		vbo.reset(new VertexBuffer(&m_vertices[0], m_vertices.size() * sizeof(VertexTexture), Dynamic_Buffer_Update));
		vbo->SetLayout(layout);
		m_VAO->AddVBO(vbo);

		ibo.reset(new IndexBuffer(&indices[0], indices.size()));
		m_VAO->AddIBO(ibo);
	}

	void TerrainEditor::ApplyNormals()
	{
		for (int i = 1; i < m_data->numvertex - 1; i++)
		{
			for (int j = 1; j < m_data->numvertex - 1; j++)
			{
				m_vertices[j + (i * m_data->numvertex)].normal = generateNormal(i, j);
			}
		}
		m_VAO->GetVertexBuffer(0)->Update(&m_vertices[0], m_vertices.size() * sizeof(VertexTexture));
	}

	void TerrainEditor::Draw()
	{
		//Renderer::BeginScene();
		//{
			//Renderer::Submit(m_VAO);
		m_VAO->Bind();
		glDrawElements(GL_TRIANGLES, m_VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT,0);
		//}
		//Renderer::EndScene();
	}

	void TerrainEditor::ApplyHeightsValueNoise(int height)
	{

		m_data->heights.resize(m_data->numvertex);
		for (int i = 0; i < m_data->numvertex; i++)
		{
			for (int j = 0; j < m_data->numvertex; j++)
			{
				float tmp_height = generateHeight(i, j) * height;
				m_data->heights[i].push_back(tmp_height);
				m_vertices[j + (i * m_data->numvertex)].vertice.y = tmp_height;

			}
		}
	
		m_VAO->GetVertexBuffer(0)->Update(&m_vertices[0], m_vertices.size() * sizeof(VertexTexture));
	}

	
	std::shared_ptr<Terrain> TerrainEditor::CreateTerrain() const
	{
		std::shared_ptr<Terrain> terrain;
		terrain.reset(new Terrain(*m_data));

		return terrain;
	}

	float TerrainEditor::getNoise(int x, int z)
	{
		std::mt19937 rng(x + z + seed);
		std::uniform_real_distribution<> dist(-1, 1);

		return dist(rng);
	}
	float TerrainEditor::getSmoothNoise(int x, int z)
	{
		float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
			+ getNoise(x + 1, z + 1)) / 16.0f;
		float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
			+ getNoise(x, z + 1)) / 8.0f;
		float center = getNoise(x, z) / 4.0f;

		return corners + sides + center;
	}
	float TerrainEditor::getInterpolatedNoise(float x, float z)
	{
		int intX = (int)x;
		int intZ = (int)z;
		float fracX = x - intX;
		float fracZ = z - intZ;


		float v1 = getSmoothNoise(intX, intZ);
		float v2 = getSmoothNoise(intX + 1, intZ);
		float v3 = getSmoothNoise(intX, intZ + 1);
		float v4 = getSmoothNoise(intX + 1, intZ + 1);
		float i1 = interpolateCosine(v1, v2, fracX);
		float i2 = interpolateCosine(v3, v4, fracX);


		return interpolateCosine(i1, i2, fracZ);
		
	}
	float TerrainEditor::interpolateCosine(float a, float b, float blend)
	{

		double theta = blend * M_PI;
		float f = (float)(1.0f - cos(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}
	float TerrainEditor::generateHeight(int x, int z)
	{
		float total = 0;
		float d = (float)pow(2, OCTAVES - 1);
		for (int i = 0; i < OCTAVES; i++) {
			float freq = (float)(pow(2, i) / d);
			float amp = (float)pow(ROUGHNESS, i) * AMPLITUDE;
			total += getInterpolatedNoise((x)* freq, (z * freq) * amp);
		}
		return total;
	}
	float TerrainEditor::getHeight(int x, int z) const
	{
		return m_data->heights[x][z];
	}
	glm::vec3 TerrainEditor::generateNormal(int x, int z) const
	{
		float heightL = getHeight(x - 1, z);
		float heightR = getHeight(x + 1, z);
		float heightD = getHeight(x, z - 1);
		float heightU = getHeight(x, z + 1);

		glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);

		return glm::normalize(normal);
	}
}