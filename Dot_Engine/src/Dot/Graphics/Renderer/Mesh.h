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


		inline const std::shared_ptr<ArrayBuffer> GetVao() { return m_VAO; }
		inline const std::string& GetFilePath() const { return m_FilePath; }
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;
		Ref<ArrayBuffer> m_VAO;


		std::string m_FilePath;
	};

	class InstancedMesh
	{
	public:
		InstancedMesh(const std::string& filename, const std::vector<glm::mat4> transforms);


		const inline unsigned int GetNum() const { return m_num; }
		const inline std::shared_ptr<ArrayBuffer> GetVao() { return mesh->GetVao(); }

	private:
		Ref<Mesh> mesh;
		unsigned int m_num;
	};
}