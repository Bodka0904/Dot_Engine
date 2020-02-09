#include "stdafx.h"
#include "InstancedMesh.h"

#include "Dot/Renderer/RenderCommand.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>


#define MAX_INSTANCE 900

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

	
	InstancedMesh::InstancedMesh(const std::string& filename, int num)
		:capacity(num), instances(0),numVertices(0)
	{
		if (capacity > MAX_INSTANCE)
		{
			capacity = MAX_INSTANCE;
			LOG_WARN("Max capacity of instanced mesh is %d", MAX_INSTANCE);
		}
		models.resize(capacity);
		for (int i = 0; i < (int)capacity; ++i)
		{
			models[i] = glm::mat4(1);		
		}
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

		vertices.reserve(mesh->mNumVertices);

		for (size_t i = 0; i < vertices.capacity(); i++)
		{
			StaticVertex vertex;
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}

			if (mesh->HasTextureCoords(0))
				vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
			vertices.push_back(vertex);
		}

		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = VertexBuffer::Create((float*)vertices.data(), (unsigned int)vertices.size() * sizeof(StaticVertex), D_STATIC_DRAW);
		numVertices = (unsigned int)vertices.size();

		BufferLayout layout = {
			{0, ShaderDataType::Float3, "position" },
			{1, ShaderDataType::Float3, "normal" },
			{2, ShaderDataType::Float3, "tangent"},
			{3, ShaderDataType::Float3, "binormal"},
			{4, ShaderDataType::Float2, "texCoord" },
		};
		vao = ArrayBuffer::Create();

		m_VertexBuffer->SetLayout(layout);
		vao->AddVBO(m_VertexBuffer);

		// Extract indices from model
		indices.reserve(mesh->mNumFaces);
		for (size_t i = 0; i < indices.capacity(); i++)
		{
			D_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
			indices.push_back({ mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] });
		}
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer = IndexBuffer::Create((void*)indices.data(), (unsigned int)indices.size() * (unsigned int)3);
		vao->AddIBO(m_IndexBuffer);



		Ref<VertexBuffer> m_VBO_MAT;
		BufferLayout mat4 = {
				{5, Dot::ShaderDataType::Mat4, "instanceModel", 1},
		};

		m_VBO_MAT = VertexBuffer::Create((float*)models.data(), capacity * sizeof(glm::mat4), D_DYNAMIC_DRAW);
		m_VBO_MAT->SetLayout(mat4);
		vao->AddVBO(m_VBO_MAT);
	}
	void InstancedMesh::Render(const Ref<Shader>& shader, int drawMod)
	{
		RenderCommand::SubmitElementInstanced(vao, instances, drawMod);
	}
	void InstancedMesh::UpdateBuffer()
	{
		vao->GetVertexBuffer(1)->Update((float*)models.data(), capacity * sizeof(glm::mat4), 0);
	}
}