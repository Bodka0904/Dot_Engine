#include "stdafx.h"
#include "AnimatedMesh.h"

#include <assert.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Dot/Renderer/RenderCommand.h"

namespace Dot {

	static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}
	namespace {
		const unsigned int ImportFlags =
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs |
			aiProcess_JoinIdenticalVertices;
	}

	void VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			if (Weights[i] == 0.0)
			{
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}
		LOG_WARN("Vertex has more than four bones/weights per vertex, discarded BoneID=%d, Weight=%f", BoneID, Weight);
	}

	SkinnedMesh::SkinnedMesh(const std::string& filename)
	{
		skeleton = NULL;
		const aiScene* pScene = NULL;
		pScene = importer.ReadFile(filename.c_str(), ImportFlags);
		if (pScene)
		{
			subMesh.resize(pScene->mNumMeshes);

			std::vector<unsigned int> indices;

			unsigned int NumVertices = 0;
			unsigned int NumIndices = 0;

			// Count the number of vertices and indices
			for (unsigned int i = 0; i < subMesh.size(); i++)
			{
				subMesh[i].NumIndices =pScene->mMeshes[i]->mNumFaces * 3;
				subMesh[i].BaseVertex = NumVertices;
				subMesh[i].BaseIndex = NumIndices;

				NumVertices += pScene->mMeshes[i]->mNumVertices;
				NumIndices += subMesh[i].NumIndices;
			}

			vertices.resize(NumVertices);
			indices.reserve(NumIndices);

			// Initialize the meshes in the scene one by one
			for (unsigned int i = 0; i < subMesh.size(); i++)
			{
				const aiMesh* paiMesh = pScene->mMeshes[i];
				initMesh(i, paiMesh, vertices, indices);
			}


			vao = ArrayBuffer::Create();
			vao->Bind();
			BufferLayout layout{
				{0, ShaderDataType::Float3, "position" },
				{1, ShaderDataType::Float3, "normal" },
				{2, ShaderDataType::Float2, "texcoord" },
				{3, ShaderDataType::Int4,	"boneID" },
				{4, ShaderDataType::Float4, "boneWeight" }
			};
			Ref<VertexBuffer> VBO = VertexBuffer::Create((float*)vertices.data(), (unsigned int)vertices.size() * sizeof(AnimatedVertex), D_DYNAMIC_DRAW);
			VBO->SetLayout(layout);
			vao->AddVBO(VBO);

			Ref<IndexBuffer> IBO = IndexBuffer::Create(&indices[0], (unsigned int)indices.size());

			vao->AddIBO(IBO);
			
		}
		else
		{
			LOG_ERR("Error parsing '%s': '%s'", filename.c_str(), importer.GetErrorString());
		}
	}

	void SkinnedMesh::SetAnimation(Ref<Animation> animation)
	{
		skeleton = animation.get();
	}

	void SkinnedMesh::Render(const Ref<Shader>& shader, int drawMod)
	{
		vao->Bind();
		for (unsigned int i = 0; i < subMesh.size(); i++)
		{
			if (skeleton != NULL)
			{
				for (size_t i = 0; i < skeleton->boneInfo.size(); i++)
				{
					std::string uniformName = std::string("u_gBones[") + std::to_string(i) + std::string("]");
					material->Set(uniformName, skeleton->boneInfo[i].FinalTransformation);
				}
			}
			RenderCommand::SubmitElementBase(subMesh[i].NumIndices, subMesh[i].BaseIndex, subMesh[i].BaseVertex, drawMod);
		}
		vao->UnBind();
	}

	void SkinnedMesh::initMesh(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<AnimatedVertex>& vertices, std::vector<unsigned int>& Indices)
	{
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
		{
			if (paiMesh->HasTextureCoords(0))
			{
				vertices[i].texcoord = glm::vec2(paiMesh->mTextureCoords[0][i].x, paiMesh->mTextureCoords[0][i].y);
			}
			vertices[i].position = glm::vec3(paiMesh->mVertices[i].x, paiMesh->mVertices[i].y, paiMesh->mVertices[i].z);
			vertices[i].normal = glm::vec3(paiMesh->mNormals[i].x, paiMesh->mNormals[i].y, paiMesh->mNormals[i].z);
		}

		std::vector<BoneInfo> BoneInfo;
		BoneInfo.resize(paiMesh->mNumBones);
		std::map<std::string, unsigned int> boneMapping;
		int numBones = 0;

		for (unsigned int i = 0; i < paiMesh->mNumBones; i++)
		{
			unsigned int BoneIndex = 0;
			std::string BoneName(paiMesh->mBones[i]->mName.data);

			if (boneMapping.find(BoneName) == boneMapping.end())
			{
				BoneIndex = numBones;
				numBones++;

				BoneInfo[BoneIndex].BoneOffset = aiMatrix4x4ToGlm(paiMesh->mBones[i]->mOffsetMatrix);
				boneMapping[BoneName] = BoneIndex;
			}
			else
			{
				BoneIndex = boneMapping[BoneName];
			}

			for (unsigned int j = 0; j < paiMesh->mBones[i]->mNumWeights; j++)
			{
				unsigned int VertexID = subMesh[MeshIndex].BaseVertex + paiMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = paiMesh->mBones[i]->mWeights[j].mWeight;
				vertices[VertexID].boneData.AddBoneData(BoneIndex, Weight);
			}
		}

		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
		{
			const aiFace& Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}
	}

	void SkinnedMesh::initAnimation(unsigned int MeshIndex, const aiMesh* paiMesh)
	{
		
	}

	Animation::Animation(const std::string& filename)
	{
		numBones = 0;
		pScene = NULL;
		pScene = importer.ReadFile(filename.c_str(), ImportFlags);
		if (pScene)
		{
			std::vector<std::string> DeleteBone;
			loadHierarchy(pScene->mRootNode, DeleteBone);
			for (auto i : DeleteBone)
			{
				boneMapping.erase(i);
			}

			for (unsigned int i = 0; i < pScene->mNumMeshes; i++)
			{
				const aiMesh* paiMesh = pScene->mMeshes[i];
				boneInfo.resize(paiMesh->mNumBones);
				for (unsigned int i = 0; i < paiMesh->mNumBones; i++)
				{
					unsigned int BoneIndex = 0;
					std::string BoneName(paiMesh->mBones[i]->mName.data);

					if (boneMapping.find(BoneName) == boneMapping.end())
					{
						BoneIndex = numBones;
						numBones++;

						boneInfo[BoneIndex].BoneOffset = aiMatrix4x4ToGlm(paiMesh->mBones[i]->mOffsetMatrix);
						boneMapping[BoneName] = BoneIndex;
					}
					else
					{
						BoneIndex = boneMapping[BoneName];
					}

				}
			}			
		}
		else
		{	
			LOG_ERR("Error parsing '%s': '%s'", filename.c_str(), importer.GetErrorString());
		}
	}


	const aiNodeAnim* Animation::findNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
	{
		for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
				return pNodeAnim;
			}
		}

		return NULL;
	}

	void Animation::loadHierarchy(const aiNode* pNode, std::vector<std::string>& boneMappingDelete)
	{
		nodeAnim[pNode->mName.data] = findNodeAnim(pScene->mAnimations[0], pNode->mName.data);

		if (boneMapping.find(pNode->mName.data) == boneMapping.end())
		{
			boneMappingDelete.push_back(pNode->mName.data);
		}
		for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
			loadHierarchy(pNode->mChildren[i], boneMappingDelete);
		}
	}

}