#include "stdafx.h"
#include "Terrain.h"



#include "Dot/Renderer/RenderSystem.h"
#include "Dot/Core/Input.h"


namespace Dot {

	Terrain::Terrain(float Size, int NumVertex)
		: size(Size), numVertex(NumVertex)
	{
		// Num vertices
		int count = numVertex * numVertex;

		// Temporary vectors 
		std::vector<int> indices;



		indices.resize(size_t(6) * (size_t(numVertex) - size_t(1)) * (size_t(numVertex) - size_t(1)));

		texCoords.resize(count);
		normals.resize(count);
		vertices.resize(count);
		

		int vertexPointer = 0;
		for (int i = 0; i < numVertex; i++)
		{
			for (int j = 0; j < numVertex; j++)
			{
				vertices[vertexPointer] = glm::vec4((float)j / ((float)numVertex - 1) * size, 0, (float)i / ((float)numVertex - 1) * size,0);
				normals[vertexPointer] = glm::vec4(0, 1, 0,0);
				texCoords[vertexPointer] = glm::vec2((float)j / ((float)numVertex - 1), (float)i / ((float)numVertex - 1));
				vertexPointer++;

			}
		}

		int pointer = 0;
		for (int gz = 0; gz < numVertex - 1; gz++) {
			for (int gx = 0; gx < numVertex - 1; gx++) {
				int topLeft = (gz * numVertex) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * numVertex) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}

		vao = ArrayBuffer::Create();
		BufferLayout layout = {
				{0, Dot::ShaderDataType::Float4, "a_Position" },

		};
		Ref<ShaderStorageBuffer> ssbo_pos = ShaderStorageBuffer::Create((float*)vertices.data(), (unsigned int)vertices.size() * sizeof(glm::vec4), D_DYNAMIC_DRAW);
		ssbo_pos->SetLayout(layout);
		vao->AddSSBO(ssbo_pos);


		BufferLayout layout_n = {
			{1, Dot::ShaderDataType::Float4, "a_Normal" }
		};
		Ref<ShaderStorageBuffer>ssbo_normal = ShaderStorageBuffer::Create((float*)normals.data(), (unsigned int)normals.size() * sizeof(glm::vec4), D_DYNAMIC_DRAW);
		ssbo_normal->SetLayout(layout_n);
		vao->AddSSBO(ssbo_normal);


		BufferLayout layout_t = {
			{2, Dot::ShaderDataType::Float2, "a_TexCoord" }
		};
		Ref<VertexBuffer>vbo_tex = VertexBuffer::Create((float*)texCoords.data(), (unsigned int)texCoords.size() * sizeof(glm::vec2), D_DYNAMIC_DRAW);
		vbo_tex->SetLayout(layout_t);
		vao->AddVBO(vbo_tex);


		Ref<IndexBuffer>ibo = IndexBuffer::Create(&indices[0], (unsigned int)indices.size());
		vao->AddIBO(ibo);
	}


	Terrain::~Terrain()
	{
	}

	void Terrain::Render(const Ref<Shader>& shader, int drawMod)
	{
		RenderCommand::SubmitElement(vao, drawMod);
	}
	
	TerrainCollider::TerrainCollider()
		: size(0),numVertex(0)
	{
		
	}
	float TerrainCollider::GetHeight(const glm::vec3& position)
	{
		float gridSquareSize = size / numVertex;

		int gridX = (int)(position.x / gridSquareSize);
		int gridZ = (int)(position.z / gridSquareSize);


		float xCoord = fmod(position.x, gridSquareSize) / gridSquareSize;
		float zCoord = fmod(position.z, gridSquareSize) / gridSquareSize;

		float answer = 0.0f;
		if (gridX >= heights.size() - 1 || gridZ >= heights.size() - 1 || gridX < 0 || gridZ < 0)
		{
			return answer;
		}
		else
		{
			if (xCoord <= 1 - zCoord)
			{
				answer = barryCentric(glm::vec3(0, (heights[size_t(gridX)][size_t(gridZ)]), 0), glm::vec3(1,
					(heights[size_t(gridX) + size_t(1)][size_t(gridZ)]), 0), glm::vec3(0,
					(heights[size_t(gridX)][size_t(gridZ) + size_t(1)]), 1), glm::vec2(xCoord, zCoord));
			}
			else
			{
				answer = barryCentric(glm::vec3(1, (heights[(size_t)gridX + (size_t)1][(size_t)gridZ]), 0), glm::vec3(1,
					(heights[size_t(gridX) + size_t(1)][(size_t)gridZ + (size_t)1]), 1), glm::vec3(0,
					(heights[size_t(gridX)][size_t(gridZ) + size_t(1)]), 1), glm::vec2(xCoord, zCoord));
			}
		}
		return answer;
	}
	void TerrainCollider::UpdateHeights(Entity entity)
	{
		auto& ter = ECSManager::Get()->GetComponent<TerrainCollider>(entity);

		heights.resize(ter.numVertex);
		for (int i = 0; i < ter.numVertex; i++)
		{
			heights[i].resize(ter.numVertex);
			for (int j = 0; j < ter.numVertex; j++)
			{
				heights[i][j] = 0;
			}
		}
		numVertex = ter.numVertex;
		size = ter.size;
	}
	float TerrainCollider::barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}
}