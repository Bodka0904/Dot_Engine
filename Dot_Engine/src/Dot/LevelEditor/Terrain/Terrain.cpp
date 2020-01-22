#include "stdafx.h"
#include "Terrain.h"



#include "Dot/Renderer/RenderSystem.h"
#include "Dot/Core/Input.h"


namespace Dot {

	Terrain::Terrain(float size, int numvertex)
		: m_Size(size), m_NumVertex(numvertex)
	{
		// Num vertices
		int count = m_NumVertex * m_NumVertex;
		
		// Temporary vectors 
		std::vector<int> indices;


	
		indices.resize(6 * (m_NumVertex - 1) * (m_NumVertex - 1));
		
		m_TexCoords.resize(count);
		m_Normals.resize(count);
		m_Vertices.resize(count);
		m_Heights.resize(m_NumVertex);

		int vertexPointer = 0;
		for (int i = 0; i < m_NumVertex; i++)
		{
			m_Heights[i].resize(m_NumVertex);
			for (int j = 0; j < m_NumVertex; j++)
			{
				m_Heights[i][j] = 0;
				m_Vertices[vertexPointer] = glm::vec4((float)j / ((float)m_NumVertex - 1) * m_Size, 0, (float)i / ((float)m_NumVertex - 1) * m_Size,0);
				m_Normals[vertexPointer] = glm::vec4(0, 1, 0,1);
				m_TexCoords[vertexPointer] = glm::vec2((float)j / ((float)m_NumVertex - 1), (float)i / ((float)m_NumVertex - 1));	
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
				{0, Dot::ShaderDataType::Float4, "a_Position" },
			
		};
		Ref<ShaderStorageBuffer> ssbo_pos = ShaderStorageBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(glm::vec4), D_DYNAMIC_DRAW);
		ssbo_pos->SetLayout(layout);
		m_VAO->AddSSBO(ssbo_pos);
	
	
		BufferLayout layout_n = {
			{1, Dot::ShaderDataType::Float4, "a_Normal" }
		};
		Ref<ShaderStorageBuffer>ssbo_normal = ShaderStorageBuffer::Create((float*)m_Normals.data(), m_Normals.size() * sizeof(glm::vec4), D_DYNAMIC_DRAW);
		ssbo_normal->SetLayout(layout_n);
		m_VAO->AddSSBO(ssbo_normal);
		
		
		BufferLayout layout_t = {
			{2, Dot::ShaderDataType::Float2, "a_TexCoord" }
		};
		Ref<VertexBuffer>vbo_tex = VertexBuffer::Create((float*)m_TexCoords.data(), m_TexCoords.size() * sizeof(glm::vec2), D_DYNAMIC_DRAW);
		vbo_tex->SetLayout(layout_t);
		m_VAO->AddVBO(vbo_tex);
	
	
		Ref<IndexBuffer>ibo = IndexBuffer::Create(&indices[0], indices.size());
		m_VAO->AddIBO(ibo);
	}

	
	Terrain::~Terrain()
	{
	}

	void Terrain::Render(const Ref<Shader>& shader, int drawMod)
	{
		RenderCommand::SubmitElement(m_VAO, drawMod);
	}

	float Terrain::GetHeight(const glm::vec3& position)
	{
		float gridSquareSize = m_Size/m_NumVertex;
	
		int gridX = (int)(position.x / gridSquareSize);
		int gridZ = (int)(position.z / gridSquareSize);
	
	
		float xCoord = fmod(position.x, gridSquareSize) / gridSquareSize;
		float zCoord = fmod(position.z, gridSquareSize) / gridSquareSize;
	
		float answer = 0.0f;
		if (gridX >= m_Heights.size() - 1 || gridZ >= m_Heights.size() - 1 || gridX < 0 || gridZ < 0)
		{
			return answer;
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
	
	

	float Terrain::barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}
}