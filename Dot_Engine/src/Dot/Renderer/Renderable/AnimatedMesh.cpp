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

	AnimatedMesh::AnimatedMesh(const std::string& Filename)
	{
		m_NumBones = 0;
		m_pScene = NULL;
		m_pScene = m_Importer.ReadFile(Filename.c_str(), ImportFlags);

		if (m_pScene)
		{
			m_IsAnimated = m_pScene->mAnimations != nullptr;
			m_SubMesh.resize(m_pScene->mNumMeshes);

			std::vector<AnimatedVertex> vertices;
			std::vector<unsigned int> indices;

			unsigned int NumVertices = 0;
			unsigned int NumIndices = 0;

			// Count the number of vertices and indices
			for (unsigned int i = 0; i < m_SubMesh.size(); i++)
			{
				m_SubMesh[i].NumIndices = m_pScene->mMeshes[i]->mNumFaces * 3;
				m_SubMesh[i].BaseVertex = NumVertices;
				m_SubMesh[i].BaseIndex = NumIndices;

				NumVertices += m_pScene->mMeshes[i]->mNumVertices;
				NumIndices += m_SubMesh[i].NumIndices;
			}

			vertices.resize(NumVertices);
			indices.reserve(NumIndices);

			// Initialize the meshes in the scene one by one
			for (unsigned int i = 0; i < m_SubMesh.size(); i++)
			{
				const aiMesh* paiMesh = m_pScene->mMeshes[i];
				initMesh(i, paiMesh, vertices, indices);
			}

			// Delete BoneMapping elements that we do not need
			std::vector<std::string> DeleteBone;
			loadHierarchy(m_pScene->mRootNode, DeleteBone);
			for (auto i : DeleteBone)
			{
				m_BoneMapping.erase(i);
			}


			m_VAO = ArrayBuffer::Create();
			m_VAO->Bind();
			BufferLayout layout{
				{0, ShaderDataType::Float3, "position" },
				{1, ShaderDataType::Float3, "normal" },
				{2, ShaderDataType::Float2, "texcoord" },
				{3, ShaderDataType::Int4,	"boneID" },
				{4, ShaderDataType::Float4, "boneWeight" }
			};
			Ref<VertexBuffer> VBO = VertexBuffer::Create((float*)vertices.data(), vertices.size() * sizeof(AnimatedVertex), D_STATIC_DRAW);
			VBO->SetLayout(layout);	
			m_VAO->AddVBO(VBO);
			
			Ref<IndexBuffer> IBO = IndexBuffer::Create(&indices[0], indices.size());
			
			m_VAO->AddIBO(IBO);
			m_InverseTransform = glm::inverse(aiMatrix4x4ToGlm(m_pScene->mRootNode->mTransformation));
		}
		else
		{
			LOG_ERR("Error parsing '%s': '%s'", Filename.c_str(), m_Importer.GetErrorString());
		}
	}

	AnimatedMesh::~AnimatedMesh()
	{
	}


	void AnimatedMesh::initMesh(unsigned int MeshIndex,
		const aiMesh* paiMesh,
		std::vector<AnimatedVertex>& vertices,
		std::vector<unsigned int>& Indices
	)
	{
		// Populate the vertex attribute vectors
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
		{
			if (paiMesh->HasTextureCoords(0))
			{
				vertices[i].texcoord = glm::vec2(paiMesh->mTextureCoords[0][i].x, paiMesh->mTextureCoords[0][i].y);
			}

			vertices[i].position = glm::vec3(paiMesh->mVertices[i].x, paiMesh->mVertices[i].y, paiMesh->mVertices[i].z);
			vertices[i].normal = glm::vec3(paiMesh->mNormals[i].x, paiMesh->mNormals[i].y, paiMesh->mNormals[i].z);	
		}

		// Load Bones
		m_BoneInfo.resize(paiMesh->mNumBones);
		for (unsigned int i = 0; i < paiMesh->mNumBones; i++)
		{
			unsigned int BoneIndex = 0;
			std::string BoneName(paiMesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) 
			{
				BoneIndex = m_NumBones;
				m_NumBones++;

				m_BoneInfo[BoneIndex].BoneOffset = aiMatrix4x4ToGlm(paiMesh->mBones[i]->mOffsetMatrix);
				m_BoneMapping[BoneName] = BoneIndex;
			}
			else
			{
				BoneIndex = m_BoneMapping[BoneName];
			}

			for (unsigned int j = 0; j < paiMesh->mBones[i]->mNumWeights; j++)
			{
				unsigned int VertexID = m_SubMesh[MeshIndex].BaseVertex + paiMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = paiMesh->mBones[i]->mWeights[j].mWeight;
				vertices[VertexID].boneData.AddBoneData(BoneIndex, Weight);
			}
		}

		// Populate the index buffer
		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) 
		{
			const aiFace& Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}
	}


	void AnimatedMesh::loadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
	{
		m_BoneInfo.resize(pMesh->mNumBones);
		for (unsigned int i = 0; i < pMesh->mNumBones; i++) 
		{
			unsigned int BoneIndex = 0;
			std::string BoneName(pMesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
				// Allocate an index for a new bone
				BoneIndex = m_NumBones;
				m_NumBones++;
				
				m_BoneInfo[BoneIndex].BoneOffset = aiMatrix4x4ToGlm(pMesh->mBones[i]->mOffsetMatrix);
				m_BoneMapping[BoneName] = BoneIndex;
			}
			else
			{
				BoneIndex = m_BoneMapping[BoneName];
			}

			for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) 
			{
				unsigned int VertexID = m_SubMesh[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
				Bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
	}


	void AnimatedMesh::Render(const Ref<Shader>& shader, int drawMod)
	{
		m_VAO->Bind();
		for (unsigned int i = 0; i < m_SubMesh.size(); i++)
		{
			for (size_t i = 0; i < m_BoneInfo.size(); i++)
			{
				std::string uniformName = std::string("u_gBones[") + std::to_string(i) + std::string("]");
				shader->UploadUniformMat4(uniformName, m_BoneInfo[i].FinalTransformation);
			}

			RenderCommand::SubmitElementBase(m_SubMesh[i].NumIndices, m_SubMesh[i].BaseIndex, m_SubMesh[i].BaseVertex, drawMod);
		}
		m_VAO->UnBind();
	}


	unsigned int AnimatedMesh::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}


	unsigned int AnimatedMesh::findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}


	unsigned int AnimatedMesh::findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) 
		{
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) 
			{
				return i;
			}
		}

		assert(0);

		return 0;
	}


	glm::vec3 AnimatedMesh::calcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) {
			auto v = pNodeAnim->mPositionKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		unsigned int PositionIndex = findPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);

		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		auto aiVec = Start + Factor * Delta;

		return { aiVec.x, aiVec.y, aiVec.z };
	}


	glm::quat AnimatedMesh::calcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			auto v = pNodeAnim->mRotationKeys[0].mValue;
			return glm::quat(v.w, v.x, v.y, v.z);
		}

		unsigned int RotationIndex = findRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		auto q = aiQuaternion();
		aiQuaternion::Interpolate(q, StartRotationQ, EndRotationQ, Factor);
		q = q.Normalize();
		return glm::quat(q.w, q.x, q.y, q.z);

	}




	glm::vec3 AnimatedMesh::calcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			auto v = pNodeAnim->mScalingKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		unsigned int ScalingIndex = findScaling(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		//assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		auto aiVec = Start + Factor * Delta;

		return { aiVec.x, aiVec.y, aiVec.z };
	}

	void AnimatedMesh::loadHierarchy(const aiNode* pNode, std::vector<std::string>& boneMappingDelete)
	{

		m_NodeAnim[pNode->mName.data] = findNodeAnim(m_pScene->mAnimations[0], pNode->mName.data);

		if (m_BoneMapping.find(pNode->mName.data) == m_BoneMapping.end())
		{
			boneMappingDelete.push_back(pNode->mName.data);
		}
		for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
			loadHierarchy(pNode->mChildren[i], boneMappingDelete);
		}



	}

	void AnimatedMesh::readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
	{

		glm::mat4 NodeTransformation(aiMatrix4x4ToGlm(pNode->mTransformation));
		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_NodeAnim[pNode->mName.data])
		{
			glm::quat RotationQ = calcInterpolatedRotation(AnimationTime, m_NodeAnim[pNode->mName.data]);
			glm::mat4 RotationM = glm::toMat4(RotationQ);

			glm::vec3 Translation = calcInterpolatedPosition(AnimationTime, m_NodeAnim[pNode->mName.data]);
			glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));;


			NodeTransformation = TranslationM * RotationM;

			GlobalTransformation = ParentTransform * NodeTransformation;

			unsigned int BoneIndex = m_BoneMapping[pNode->mName.data];
			m_BoneInfo[BoneIndex].FinalTransformation = GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;

		}

		for (unsigned int i = 0; i < pNode->mNumChildren; i++) 
		{
			readNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
		}
	}


	void AnimatedMesh::AnimateBones(float TimeInSeconds)
	{
		m_Time += TimeInSeconds;

		float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = m_Time * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);
		if (AnimationTime >= m_pScene->mAnimations[0]->mDuration)
			m_Time = 0;

		readNodeHeirarchy(AnimationTime, m_pScene->mRootNode, glm::mat4(1.0));
	}

	void AnimatedMesh::SetToDefaultPosition()
	{
		if (m_Time > 0.0f)
		{
			m_Time = 0;
			float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
			float TimeInTicks = m_Time * TicksPerSecond;
			float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

			readNodeHeirarchy(AnimationTime, m_pScene->mRootNode, glm::mat4(1.0));
		}
	}

	const aiNodeAnim* AnimatedMesh::findNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
	{
		for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
				return pNodeAnim;
			}
		}

		return NULL;
	}

}