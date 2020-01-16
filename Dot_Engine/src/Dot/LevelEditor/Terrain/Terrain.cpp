#include "stdafx.h"
#include "Terrain.h"

#include "Dot/Utils/Image/ImageLoader.h"
#include "Dot/Utils/Random/RandomGenerator.h"


#include "Dot/Renderer/RenderSystem.h"
#include "Dot/Core/Input.h"

#include "Dot/Gui/GuiSystem/Gui.h"
#include "Dot/Gui/GuiSystem/Slider.h"
#include "Dot/Gui/GuiSystem/Button.h"
#include "Dot/Gui/GuiSystem/Arrowbutton.h"
#include "Dot/Gui/GuiSystem/Checkbox.h"
#include "Dot/Gui/GuiSystem/Dropdown.h"

#define _USE_MATH_DEFINES
#include <math.h>




namespace Dot {

	Terrain::Terrain(float size, int numvertex)
		: m_Size(size), m_NumVertex(numvertex)
	{
		// Num vertices
		int count = m_NumVertex * m_NumVertex;
		
		// Temporary vectors 
		std::vector<int> indices;
		std::vector<glm::vec2> texcoords;
		std::vector<glm::vec3> normals;

		texcoords.resize(count);
		normals.resize(count);
		indices.resize(6 * (m_NumVertex - 1) * (m_NumVertex - 1));
	
		m_Vertices.resize(count);
		m_Heights.resize(m_NumVertex);

		int vertexPointer = 0;
		for (int i = 0; i < m_NumVertex; i++)
		{
			m_Heights[i].resize(m_NumVertex);
			for (int j = 0; j < m_NumVertex; j++)
			{
				m_Heights[i][j] = 0;
				m_Vertices[vertexPointer] = glm::vec3((float)j / ((float)m_NumVertex - 1) * m_Size, 0, (float)i / ((float)m_NumVertex - 1) * m_Size);
				normals[vertexPointer] = glm::vec3(0, 1, 0);
				texcoords[vertexPointer] = glm::vec2((float)j / ((float)m_NumVertex - 1), (float)i / ((float)m_NumVertex - 1));	
				vertexPointer++;
	
			}
		}
		
		int pointer = 0;
		for (int gz = 0; gz < m_NumVertex - 1; gz++) {
			for (int gx = 0; gx < m_NumVertex - 1; gx++) {
				int topLeft = (gz * m_NumVertex) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * m_NumVertex) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}
	
		m_VAO = ArrayBuffer::Create();
		BufferLayout layout = {
				{0, Dot::ShaderDataType::Float3, "a_Position" },
			
		};
		Ref<VertexBuffer>vbo_pos = VertexBuffer::Create((void*)&m_Vertices[0], m_Vertices.size() * sizeof(glm::vec3), D_DYNAMIC_DRAW);
		vbo_pos->SetLayout(layout);
		m_VAO->AddVBO(vbo_pos);
	
	
		BufferLayout layout_n = {
			{1, Dot::ShaderDataType::Float3, "a_Normal" }
		};
		Ref<VertexBuffer>vbo_normal = VertexBuffer::Create((void*)&normals[0], normals.size() * sizeof(glm::vec3), D_DYNAMIC_DRAW);
		vbo_normal->SetLayout(layout_n);
		m_VAO->AddVBO(vbo_normal);
		
		
		BufferLayout layout_t = {
			{2, Dot::ShaderDataType::Float2, "a_TexCoord" }
		};
		Ref<VertexBuffer>vbo_tex = VertexBuffer::Create((void*)&texcoords[0], texcoords.size() * sizeof(glm::vec2), D_DYNAMIC_DRAW);
		vbo_tex->SetLayout(layout_t);
		m_VAO->AddVBO(vbo_tex);
	
	
		Ref<IndexBuffer>ibo = IndexBuffer::Create(&indices[0], indices.size());
		m_VAO->AddIBO(ibo);


	}

	Terrain::Terrain(const std::string& heightMap, float size, float height)
	{
		_BMPIMAGE img = ImageLoader::LoadBMP(heightMap);// Num vertices
		m_NumVertex = img.width;
		m_Height = height;
		m_Size = size;
		if (img.width != img.height)
		{
			LOG_WARN("Only heightmaps with same width and height supported");
		}

		int count = m_NumVertex * m_NumVertex;

		// Temporary vectors 
		std::vector<glm::vec2> texcoords;
		std::vector<glm::vec3> normals;
		std::vector<int > indices;

		texcoords.resize(count);
		normals.resize(count);
		indices.resize(6 * (m_NumVertex - 1) * (m_NumVertex - 1));

		m_Vertices.resize(count);
		m_Heights.resize(m_NumVertex);

		int vertexPointer = 0;
		for (int i = 0; i < m_NumVertex; ++i)
		{
			m_Heights[i].resize(m_NumVertex);
			for (int j = 0; j < m_NumVertex; ++j)
			{
				m_Heights[i][j] = (float)img.pixels[i][j].r / 255.0f * m_Height;	
				m_Vertices[vertexPointer] = glm::vec3((float)i / ((float)m_NumVertex - 1) * m_Size, m_Heights[i][j], (float)j / ((float)m_NumVertex - 1) * m_Size);
				normals[vertexPointer] = glm::vec3(0,1,0);
				texcoords[vertexPointer] = glm::vec2((float)i / ((float)m_NumVertex - 1), (float)j / ((float)m_NumVertex - 1));
				
				vertexPointer++;
			}
		}

		vertexPointer = 0;
		for (int j = 0; j < m_Heights.size() - 1; ++j)
		{
			for (int i = 0; i < m_Heights.size() - 1; ++i)
			{
				int start = j * m_Heights.size() + i;
				indices[vertexPointer]=(start);
				indices[vertexPointer+1]=(start + 1);
				indices[vertexPointer+2]=(start + m_Heights.size());

				indices[vertexPointer+3]=(start + 1);
				indices[vertexPointer+4]=(start + 1 + m_Heights.size());
				indices[vertexPointer+5]=(start + m_Heights.size());
				vertexPointer += 6;
			}
		}

		m_VAO = ArrayBuffer::Create();
		BufferLayout layout = {
				{0, Dot::ShaderDataType::Float3, "a_Position" },

		};
		Ref<VertexBuffer>vbo_pos = VertexBuffer::Create((void*)&m_Vertices[0], m_Vertices.size() * sizeof(glm::vec3), D_DYNAMIC_DRAW);
		vbo_pos->SetLayout(layout);
		m_VAO->AddVBO(vbo_pos);
		
		BufferLayout layout_n = {
			{1, Dot::ShaderDataType::Float3, "a_Normal" }
		};
		Ref<VertexBuffer>vbo_normal = VertexBuffer::Create((void*)&normals[0], normals.size() * sizeof(glm::vec3), D_DYNAMIC_DRAW);
		vbo_normal->SetLayout(layout_n);
		m_VAO->AddVBO(vbo_normal);


		BufferLayout layout_t = {
			{2, Dot::ShaderDataType::Float2, "a_TexCoord" }
		};
		Ref<VertexBuffer>vbo_tex = VertexBuffer::Create((void*)&texcoords[0], texcoords.size() * sizeof(glm::vec2), D_DYNAMIC_DRAW);
		vbo_tex->SetLayout(layout_t);
		m_VAO->AddVBO(vbo_tex);
		
		Ref<IndexBuffer>ibo = IndexBuffer::Create(&indices[0], indices.size());
		m_VAO->AddIBO(ibo);
	}
	
	Terrain::~Terrain()
	{
	}

	void Terrain::DiamondSquare()
	{
		diamondSquare(m_NumVertex);
	}
	
	void Terrain::HeightsValueNoise()
	{
		int vertexPointer = 0;
		for (int i = 0; i < m_NumVertex; i++)
		{
			for (int j = 0; j < m_NumVertex; j++)
			{
				float height = generateHeight(i, j) * m_Height;
				m_Heights[j][i] = height;
			}
		}
	}
	
	void Terrain::ApplyNormals()
	{
		int vertexPointer = 0;
		std::vector<glm::vec3> normals;
		normals.resize(m_NumVertex * m_NumVertex);

		for (int i = 0; i < m_NumVertex ; i++)
		{
			for (int j = 0; j < m_NumVertex; j++)
			{
				normals[vertexPointer] = generateNormal(j, i);
				vertexPointer++;
			}
		}
		m_VAO->GetVertexBuffer(1)->Update(&normals[0], normals.size() * sizeof(glm::vec3), 0);
	}

	void Terrain::ApplyHeights()
	{
		int vertexPointer = 0;
		for (int i = 0; i < m_NumVertex; i++)
		{
			for (int j = 0; j < m_NumVertex; j++)
			{
				m_Vertices[vertexPointer].y = m_Heights[j][i];
				vertexPointer++;
			}
		}
		m_VAO->GetVertexBuffer(0)->Update(&m_Vertices[0], m_Vertices.size() * sizeof(glm::vec3), 0);
	}

	
	float Terrain::GetHeight(const glm::vec3& position)
	{
		float gridSquareSize = m_Size/m_NumVertex;
	
		int gridX = (int)(position.x / gridSquareSize);
		int gridZ = (int)(position.z / gridSquareSize);
	
	
		float xCoord = fmod(position.x, gridSquareSize) / gridSquareSize;
		float zCoord = fmod(position.z, gridSquareSize) / gridSquareSize;
	
		float answer;
		if (gridX >= m_Heights.size() - 1 || gridZ >= m_Heights.size() - 1 || gridX < 0 || gridZ < 0)
		{
			return position.y;
		}
		else
		{
			if (xCoord <= 1 - zCoord)
			{
				answer = barryCentric(glm::vec3(0, (m_Heights[gridX][gridZ]), 0), glm::vec3(1,
					(m_Heights[gridX + 1][gridZ]), 0), glm::vec3(0,
					(m_Heights[gridX][gridZ + 1]), 1), glm::vec2(xCoord, zCoord));			
			}
			else
			{
				answer = barryCentric(glm::vec3(1, (m_Heights[gridX + 1][gridZ]), 0), glm::vec3(1,
					(m_Heights[gridX + 1][gridZ + 1]), 1), glm::vec3(0,
					(m_Heights[gridX][gridZ + 1]), 1), glm::vec2(xCoord, zCoord));
			}
		}	
		return answer;	
	}
	
	float Terrain::getNoise(int x, int z)
	{
		std::mt19937 rng(x + z + m_Seed);
		std::uniform_real_distribution<> dist(-1, 1);

		return dist(rng);
	}
	
	float Terrain::getSmoothNoise(int x, int z)
	{
		float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
			+ getNoise(x + 1, z + 1)) / 16.0f;
		float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
			+ getNoise(x, z + 1)) / 8.0f;
		float center = getNoise(x, z) / 4.0f;
	
		return corners + sides + center;
	}
	
	float Terrain::getInterpolatedNoise(float x, float z)
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
	
	float Terrain::interpolateCosine(float a, float b, float blend)
	{
		double theta = blend * M_PI;
		float f = (float)(1.0f - cos(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}
	
	float Terrain::generateHeight(int x, int z)
	{
		float total = 0;
		float d = (float)pow(2, m_Octaves - 1);
		for (int i = 0; i < m_Octaves; i++) {
			float freq = (float)(pow(2, i) / d);
			float amp = (float)pow(m_Roughness, i) * m_Amplitude;
			total += getInterpolatedNoise((x)* freq, (z * freq) * amp);
		}
		return total;
	}
	
	float Terrain::getHeight(int x, int z) const
	{
		if (x < 0 || x >= m_Heights.size() || z < 0 || z >= m_Heights[0].size())
		{
			return 0;
		}
		else
		{
			return m_Heights[x][z];
		}
	}
	
	glm::vec3 Terrain::generateNormal(int x, int z) const
	{
		float heightL = getHeight(x - 1, z);
		float heightR = getHeight(x + 1, z);
		float heightD = getHeight(x, z - 1);
		float heightU = getHeight(x, z + 1);
	
		glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);
	
		return glm::normalize(normal);
	}
	
	void Terrain::diamondSquare(int size)
	{
		int half = size / 2;
		if (half < 1)
			return;

		for (int z = half; z < m_NumVertex; z += size)
		{
			for (int x = half; x < m_NumVertex; x += size)
			{
				squareStep(x % m_NumVertex, z % m_NumVertex, half);
			}
		}

		int col = 0;
		for (int x = 0; x < m_NumVertex; x += half)
		{
			col++;
			if (col % 2 == 1)
			{
				for (int z = half; z < m_NumVertex; z += size)
				{
					diamondStep(x % m_NumVertex, z % m_NumVertex, half);
				}
			}
			else
			{
				for (int z = 0; z < m_NumVertex; z += size)
				{
					diamondStep(x % m_NumVertex, z % m_NumVertex, half);
				}
			}
		}
		diamondSquare(size / 2);
	}
	void Terrain::squareStep(int x, int z, int reach)
	{
		int count = 0;
		float avg = 0.0f;
		if (x - reach >= 0 && z - reach >= 0)
		{
			avg += m_Heights[x - reach][z - reach];
			count++;
		}
		if (x - reach >= 0 && z + reach < m_NumVertex)
		{
			avg += m_Heights[x - reach][z + reach];
			count++;
		}
		if (x + reach < m_NumVertex && z - reach >= 0)
		{
			avg += m_Heights[x + reach][z - reach];
			count++;
		}
		if (x + reach < m_NumVertex && z + reach < m_NumVertex)
		{
			avg += m_Heights[x + reach][z + reach];
			count++;
		}
		avg += RandomGenerator::Random(double(reach) / 2, reach) * m_Height;
		avg /= count;
		m_Heights[x][z] = round(avg);
	}

	void Terrain::diamondStep(int x, int z, int reach)
	{
		int count = 0;
		float avg = 0.0f;
		if (x - reach >= 0)
		{
			avg += m_Heights[x - reach][z];
			count++;
		}
		if (x + reach < m_NumVertex)
		{
			avg += m_Heights[x + reach][z];
			count++;
		}
		if (z - reach >= 0)
		{
			avg += m_Heights[x][z - reach];
			count++;
		}
		if (z + reach < m_NumVertex)
		{
			avg += m_Heights[x][z + reach];
			count++;
		}
		avg += RandomGenerator::Random(double(reach) / 2, reach) * m_Height;
		avg /= count;
		m_Heights[x][z] = (int)avg;
	}

	float Terrain::barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	TerrainBrush::TerrainBrush(float radius, Ref<Terrain> terrain)
		:
		m_Radius(radius),m_Strength(0.0f),m_Smooth(0.0f)
	{
		D_ASSERT(Gui::Get(), "Class require GUI initialized");
		m_Terrain = terrain;
		m_Entity = ECSManager::Get()->CreateEntity();
		ECSManager::Get()->AddComponent(m_Entity, Transform{ glm::vec3(0, 0, 0),glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) });
		ECSManager::Get()->AddComponent(m_Entity, Dot::RigidBody{ glm::vec3{0, 0, 0} });
		m_Transform = &ECSManager::Get()->GetComponent<Transform>(m_Entity);


		Wrapper::Create("Terrain Brush", glm::vec2(Input::GetWindowSize().first - 200, 0), glm::vec2(200, 200), 10);
		Gui::Get()->EnableWrapper("Terrain Brush");
		{
			Slider::Create("Radius", glm::vec2(300, 300), glm::vec2(200, 20), &m_Radius,0, 50);
			Slider::Create("Strength", glm::vec2(300, 300), glm::vec2(200, 20), &m_Strength,-10, 10);
			Slider::Create("Smooth factor", glm::vec2(300, 300), glm::vec2(200, 20), &m_Smooth, 0, 1);
			Checkbox::Create("Smooth", glm::vec2(300, 300), glm::vec2(50, 50));
		}
		Gui::Get()->DisableWrapper();

	}
	TerrainBrush::~TerrainBrush()
	{
		ECSManager::Get()->DestroyEntity(m_Entity);
	}

	void Dot::TerrainBrush::ApplyBrush()
	{	
		glm::vec2 indices;
		indices.x = m_Transform->pos.x / (m_Terrain->m_Size / m_Terrain->m_NumVertex);
		indices.y = m_Transform->pos.z / (m_Terrain->m_Size / m_Terrain->m_NumVertex);

		int indexX = (int)floor(indices.x);
		int indexY = (int)floor(indices.y);
		
		int maxX = 0;
		int minX = 0;
		int maxY = 0;
		int minY = 0;

		float range = m_Radius/2;
		for (int y = -range; y <= range; y++)
			for (int x = -range; x <= range; x++)
				if (x * x + y * y <= range * range)
				{
					if (indexX + x < m_Terrain->m_Heights.size()-1 
					 && indexY + y < m_Terrain->m_Heights[indexX + x].size()-1
					 && indexX + x > 1 && indexY + y > 1)
					{
						if (Checkbox::GetWrapped("Terrain Brush", "Smooth").GetClicked())
						{
							m_Terrain->m_Heights[indexX + x][indexY + y] = smoothPoint(indexX + x, indexY + y);
						}
						else
							m_Terrain->m_Heights[indexX + x][indexY + y] += m_Strength;
						
						if (indexX + x > maxX)
							maxX = indexX + x;
						if (indexY + y > maxY)
							maxY = indexY + y;
					}
				}

		minX = maxX - (int)m_Radius;
		minY = maxY - (int)m_Radius;
		
		if (minX < 0)
			minX = 0;
		if (minY < 0)
			minY = 0;

		int counter = 0;
		for (int i = minX; i < maxX; ++i)
		{
			counter += m_Terrain->m_NumVertex;
			for (int j = minY; j < maxY; ++j)
			{
				m_Terrain->m_Vertices[i+(j*m_Terrain->m_NumVertex)].y = m_Terrain->m_Heights[i][j];
			}
		}

		float updateRange = minX + (minY * m_Terrain->m_NumVertex);
		m_Terrain->m_VAO->GetVertexBuffer(0)->Update(&m_Terrain->m_Vertices[updateRange], counter * sizeof(glm::vec3), updateRange * sizeof(glm::vec3));		
	}

	float Dot::TerrainBrush::smoothPoint(int x, int z)
	{
		float left = m_Terrain->m_Heights[x-1][z];
		float right = m_Terrain->m_Heights[x+1][z];
		float bottom = m_Terrain->m_Heights[x][z-1];
		float top = m_Terrain->m_Heights[x][z+1];

		return (left + right + bottom + top) / 4;
	}

	
}