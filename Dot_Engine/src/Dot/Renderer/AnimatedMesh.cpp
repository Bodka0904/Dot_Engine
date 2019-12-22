#include "stdafx.h"
#include "AnimatedMesh.h"

#include <assert.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "RenderCommand.h"

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

	void AnimatedMesh::VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++) {
			if (Weights[i] == 0.0) {
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}
	}

	AnimatedMesh::AnimatedMesh(const std::string& Filename)
	{
		m_NumBones = 0;
		m_pScene = NULL;

		m_pScene = m_Importer.ReadFile(Filename.c_str(), ImportFlags);
		if (m_pScene) 
		{
			InitFromScene(m_pScene, Filename);
			m_InverseTransform = glm::inverse(aiMatrix4x4ToGlm(m_pScene->mRootNode->mTransformation));
		}
		else 
		{
			LOG_ERR("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
		}
	}

	AnimatedMesh::~AnimatedMesh()
	{
	}


	bool AnimatedMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
	{
		m_Entries.resize(pScene->mNumMeshes);
	
		
		std::vector<glm::vec3> Positions;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec2> TexCoords;
		std::vector<VertexBoneData> Bones;
		
		std::vector<unsigned int> Indices;

		unsigned int NumVertices = 0;
		unsigned int NumIndices = 0;

		// Count the number of vertices and indices
		for (unsigned int i = 0; i < m_Entries.size(); i++) {
			m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
			m_Entries[i].BaseVertex = NumVertices;
			m_Entries[i].BaseIndex = NumIndices;

			NumVertices += pScene->mMeshes[i]->mNumVertices;
			NumIndices += m_Entries[i].NumIndices;
		}

		// Reserve space in the vectors for the vertex attributes and indices
		Positions.reserve(NumVertices);
		Normals.reserve(NumVertices);
		TexCoords.reserve(NumVertices);
		Bones.resize(NumVertices);
		Indices.reserve(NumIndices);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0; i < m_Entries.size(); i++) {
			const aiMesh* paiMesh = pScene->mMeshes[i];
			InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
		}

		// Delete BoneMapping elements that we do not need
		std::vector<std::string> DeleteBone;
		LoadHierarchy(m_pScene->mRootNode,DeleteBone);
		for (auto i : DeleteBone)
		{
			m_BoneMapping.erase(i);
		}
		
		m_VAO = ArrayBuffer::Create();
		m_VAO->Bind();
		BufferLayout layoutpos = {
			{0, ShaderDataType::Float3, "position" },
		};
		Ref<VertexBuffer> m_PosBuffer = VertexBuffer::Create((void*)&Positions[0], sizeof(Positions[0]) * Positions.size(),D_STATIC_DRAW);
		m_PosBuffer->SetLayout(layoutpos);
		
		BufferLayout layouttex = {
			{1, ShaderDataType::Float2, "texcoord" },
		};
		Ref<VertexBuffer> m_TexCoordBuffer = VertexBuffer::Create((void*)& TexCoords[0], sizeof(TexCoords[0]) * TexCoords.size(), D_STATIC_DRAW);
		m_TexCoordBuffer->SetLayout(layouttex);
		
		BufferLayout layoutnormal = {
			{2, ShaderDataType::Float3, "normal" },
		};
		Ref<VertexBuffer> m_NormalBuffer = VertexBuffer::Create((void*)& Normals[0], sizeof(Normals[0]) * Normals.size(), D_STATIC_DRAW);
		m_NormalBuffer->SetLayout(layoutnormal);
		
		BufferLayout layoutbone = {
			{3, ShaderDataType::Int4, "boneID" },
			{4, ShaderDataType::Float4, "boneWeight" },
		};
		Ref<VertexBuffer> m_BoneBuffer = VertexBuffer::Create((void*)& Bones[0], sizeof(Bones[0]) * Bones.size(), D_STATIC_DRAW);
		m_BoneBuffer->SetLayout(layoutbone);
		
		Ref<IndexBuffer> m_IndexBuffer = IndexBuffer::Create(&Indices[0], Indices.size());

		m_VAO->AddVBO(m_PosBuffer);
		m_VAO->AddVBO(m_TexCoordBuffer);
		m_VAO->AddVBO(m_NormalBuffer);
		m_VAO->AddVBO(m_BoneBuffer);
		m_VAO->AddIBO(m_IndexBuffer);
		
		m_VAO->UnBind();
		return true;
	};


	void AnimatedMesh::InitMesh(unsigned int MeshIndex,
		const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<VertexBoneData>& Bones,
		std::vector<unsigned int>& Indices
		)
	{
		// Populate the vertex attribute vectors
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
			
			if (paiMesh->HasTextureCoords(0))
			{
				TexCoords.push_back(glm::vec2(paiMesh->mTextureCoords[0][i].x, paiMesh->mTextureCoords[0][i].y));
			}
			Positions.push_back(glm::vec3(paiMesh->mVertices[i].x, paiMesh->mVertices[i].y, paiMesh->mVertices[i].z));
			Normals.push_back(glm::vec3(paiMesh->mNormals[i].x, paiMesh->mNormals[i].y, paiMesh->mNormals[i].z));
			
		}

		LoadBones(MeshIndex, paiMesh, Bones);

		// Populate the index buffer
		for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
			const aiFace& Face = paiMesh->mFaces[i];
			assert(Face.mNumIndices == 3);
			Indices.push_back(Face.mIndices[0]);
			Indices.push_back(Face.mIndices[1]);
			Indices.push_back(Face.mIndices[2]);
		}
	}


	void AnimatedMesh::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
	{
		m_FinalTransformation.resize(pMesh->mNumBones);
		for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
			unsigned int BoneIndex = 0;
			std::string BoneName(pMesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
				// Allocate an index for a new bone
				BoneIndex = m_NumBones;
				m_NumBones++;
				BoneInfo bi;
				m_BoneInfo.push_back(bi);
				m_BoneInfo[BoneIndex].BoneOffset = aiMatrix4x4ToGlm(pMesh->mBones[i]->mOffsetMatrix);
				m_BoneMapping[BoneName] = BoneIndex;
				
			}
			else {
				BoneIndex = m_BoneMapping[BoneName];
			}

			for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
				unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
				Bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
	}


	void AnimatedMesh::Render()
	{
		m_VAO->Bind();
		for (unsigned int i = 0; i < m_Entries.size(); i++) 
		{
			RenderCommand::SubmitElementBase(m_Entries[i].NumIndices, m_Entries[i].BaseIndex, m_Entries[i].BaseVertex, D_TRIANGLES);
		}
		m_VAO->UnBind();
	}


	unsigned int AnimatedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);

		return 0;
	}


	unsigned int AnimatedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);

		return 0;
	}


	unsigned int AnimatedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);

		return 0;
	}


	glm::vec3 AnimatedMesh::CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) {
			auto v = pNodeAnim->mPositionKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
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


	glm::quat AnimatedMesh::CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			auto v = pNodeAnim->mRotationKeys[0].mValue;
			return glm::quat(v.w, v.x, v.y, v.z);
		}

		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
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
	

	

	glm::vec3 AnimatedMesh::CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			auto v = pNodeAnim->mScalingKeys[0].mValue;
			return { v.x, v.y, v.z };
		}

		unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
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

	void AnimatedMesh::LoadHierarchy(const aiNode* pNode, std::vector<std::string>& boneMappingDelete)
	{

		m_NodeAnim[pNode->mName.data] = FindNodeAnim(m_pScene->mAnimations[0], pNode->mName.data);
		
		if (m_BoneMapping.find(pNode->mName.data) == m_BoneMapping.end())
		{
			boneMappingDelete.push_back(pNode->mName.data);
		}
		for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
			LoadHierarchy(pNode->mChildren[i],boneMappingDelete);
		}
		
		

	}

	void AnimatedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
	{

		glm::mat4 NodeTransformation(aiMatrix4x4ToGlm(pNode->mTransformation));
		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_NodeAnim[pNode->mName.data]) 
		{	
			glm::quat RotationQ = CalcInterpolatedRotation(AnimationTime, m_NodeAnim[pNode->mName.data]);
			glm::mat4 RotationM = glm::toMat4(RotationQ);
			
			glm::vec3 Translation = CalcInterpolatedPosition(AnimationTime, m_NodeAnim[pNode->mName.data]);
			glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));;
			

			NodeTransformation = TranslationM * RotationM;
			
			GlobalTransformation = ParentTransform * NodeTransformation;

			unsigned int BoneIndex = m_BoneMapping[pNode->mName.data];
			m_FinalTransformation[BoneIndex] = GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;

		}

		for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
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

		ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, glm::mat4(1.0));
	}

	void AnimatedMesh::SetToDefaultPosition()
	{
		if (m_Time > 0.0f)
		{
			m_Time = 0;
			float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
			float TimeInTicks = m_Time * TicksPerSecond;
			float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

			ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, glm::mat4(1.0));
		}	
	}

	const aiNodeAnim* AnimatedMesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
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