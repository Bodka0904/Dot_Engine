#include "stdafx.h"
#include "StaticMesh.h"

#include "Dot/Renderer/RenderCommand.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace Dot {
	namespace {
		const unsigned int ImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_PreTransformVertices |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_Debone |
			aiProcess_ValidateDataStructure;
	}

	struct LogStream : public Assimp::LogStream
	{
		static void Initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
			}
		}

		void write(const char* message) override
		{
			LOG_ERR("Not inited: %s", message);
		}
	};

	StaticMesh::StaticMesh(const std::string& filename)
	{
		LogStream::Initialize();

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filename, ImportFlags);
		if (!scene || !scene->HasMeshes())
		{
			LOG_ERR("Failed to load mesh file: %s", filename.c_str());
		}
		aiMesh* mesh = scene->mMeshes[0];

		D_ASSERT(mesh->HasPositions(), "Meshes require positions.");
		D_ASSERT(mesh->HasNormals(), "Meshes require normals.");

		m_Vertices.reserve(mesh->mNumVertices);

		// Extract vertices from model
		m_Min.x = mesh->mVertices[0].x;
		m_Min.y = mesh->mVertices[0].y;
		m_Min.z = mesh->mVertices[0].z;

		m_Max.x = mesh->mVertices[0].x;
		m_Max.y = mesh->mVertices[0].y;
		m_Max.z = mesh->mVertices[0].z;
		for (size_t i = 0; i < m_Vertices.capacity(); i++)
		{
			StaticVertex vertex;
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (vertex.Position.x < m_Min.x)
				m_Min.x = vertex.Position.x;
			if (vertex.Position.y < m_Min.y)
				m_Min.y = vertex.Position.y;
			if (vertex.Position.z < m_Min.z)
				m_Min.z = vertex.Position.z;

			if (vertex.Position.x > m_Max.x)
				m_Max.x = vertex.Position.x;
			if (vertex.Position.y > m_Max.y)
				m_Max.y = vertex.Position.y;
			if (vertex.Position.z > m_Max.z)
				m_Max.z = vertex.Position.z;

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}

			if (mesh->HasTextureCoords(0))
				vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			m_Vertices.push_back(vertex);
		}

		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = VertexBuffer::Create((void*)m_Vertices.data(), m_Vertices.size() * sizeof(StaticVertex), D_STATIC_DRAW);
		m_NumVertices = m_Vertices.size();

		BufferLayout layout = {
			{0, ShaderDataType::Float3, "position" },
			{1, ShaderDataType::Float3, "normal" },
			{2, ShaderDataType::Float3, "tangent"},
			{3, ShaderDataType::Float3, "binormal"},
			{4, ShaderDataType::Float2, "texCoord" },
		};
		m_VAO = ArrayBuffer::Create();

		m_VertexBuffer->SetLayout(layout);
		m_VAO->AddVBO(m_VertexBuffer);

		// Extract indices from model
		m_Indices.reserve(mesh->mNumFaces);
		for (size_t i = 0; i < m_Indices.capacity(); i++)
		{
			D_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
			m_Indices.push_back({ mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] });
		}
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer = IndexBuffer::Create((void*)m_Indices.data(), m_Indices.size() * 3);
		m_VAO->AddIBO(m_IndexBuffer);

		m_Vertices.clear();
		m_Indices.clear();
	}

	StaticMesh::~StaticMesh()
	{
	}

	void StaticMesh::Render(const Ref<Shader>& shader, int drawMod)
	{
		RenderCommand::SubmitElement(m_VAO, drawMod);
	}


	Mesh::Mesh(const Ref<ArrayBuffer> vao)
	{
		m_VAO = vao;
	}

	void Mesh::Render(const Ref<Shader>& shader, int drawMod)
	{
		RenderCommand::SubmitElement(m_VAO, drawMod);
	}

}