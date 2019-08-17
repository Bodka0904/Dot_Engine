#include "stdafx.h"
#include "Mesh.h"

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

	Mesh::Mesh(const std::string& filename)
		: m_FilePath(filename)
	{
		LogStream::Initialize();

		LOG_INFO("Loading mesh: %s", filename.c_str());

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
		for (size_t i = 0; i < m_Vertices.capacity(); i++)
		{
			Vertex vertex;
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

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
		m_VertexBuffer = std::make_shared<VertexBuffer>((void*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex),Static_Buffer_Update);
		
		BufferLayout layout = {
			{0, ShaderDataType::Float3, "position" },
			{1, ShaderDataType::Float3, "normal" },
			{2, ShaderDataType::Float3, "tangent"},
			{3, ShaderDataType::Float3, "binormal"},
			{4, ShaderDataType::Float2, "texCoord" },
		};
		m_VAO = std::make_shared<ArrayBuffer>();

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
		m_IndexBuffer = std::make_shared<IndexBuffer>((void*)m_Indices.data(), m_Indices.size() * 3);

		
		m_VAO->AddIBO(m_IndexBuffer);

	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Render()
	{
	
		
	}


	InstancedMesh::InstancedMesh(const std::string& filename, const std::vector<glm::mat4> transforms)
		: m_num(transforms.size())
	{
		mesh = std::make_shared<Mesh>(filename);


		Ref<VertexBuffer> m_VBO_MAT;
		BufferLayout mat4 = {
				{5, Dot::ShaderDataType::Mat4, "instanceModel", 1},
		};

		m_VBO_MAT.reset((new VertexBuffer(&transforms[0], m_num * sizeof(glm::mat4), Dynamic_Buffer_Update)));
		m_VBO_MAT->SetLayout(mat4);
		mesh->GetVao()->AddVBO(m_VBO_MAT);
	}


}