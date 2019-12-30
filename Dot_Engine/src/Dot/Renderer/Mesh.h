#pragma once
#include <glm/glm.hpp>

#include "Buffers/ArrayBuffer.h"

namespace Dot {
	class Mesh
	{
	public:
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec3 Tangent;
			glm::vec3 Binormal;
			glm::vec2 Texcoord;
		};
		static_assert(sizeof(Vertex) == 14 * sizeof(float));
		static const int NumAttributes = 5;

		struct Index
		{
			uint32_t V1, V2, V3;
		};
		static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

		Mesh(const std::string& filename);
		~Mesh();

		inline const glm::vec3& GetMin() const { return m_Min; }
		inline const glm::vec3& GetMax() const { return m_Max; }
		inline const Ref<ArrayBuffer>& GetVAO() { return m_VAO; }
		inline const std::string& GetFilePath() const { return m_FilePath; }
		inline const unsigned int GetNumVertices() { return m_NumVertices; }
	private:
		Ref<ArrayBuffer> m_VAO;
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;
		unsigned int m_NumVertices;
	
		std::string m_FilePath;
	private:
		glm::vec3 m_Min;
		glm::vec3 m_Max;
	};

	class InstancedMesh
	{
	public:
		InstancedMesh(const std::string& name,int num, const std::vector<glm::mat4>& transforms);
		void Update(const std::vector<glm::mat4>& transforms, unsigned int numInstances, unsigned int offsetInstances);
		
		const inline unsigned int GetNumVertices() const { return m_Mesh->GetNumVertices(); }
		const inline unsigned int GetCapacity() const { return m_Capacity; }
		const inline unsigned int GetNumInstance() const { return m_Instances; }
		const inline Ref<Mesh>& GetMesh() { return m_Mesh; }
	private:
		Ref<Mesh> m_Mesh;
		unsigned int m_Capacity;
		unsigned int m_Instances;
	};
}