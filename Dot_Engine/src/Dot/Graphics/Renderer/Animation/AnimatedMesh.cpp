#include "stdafx.h"
#include "AnimatedMesh.h"

#include <assert.h>
#include <GL/glew.h>


namespace Dot {
	

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
		memset(m_Buffers, 0, sizeof(m_Buffers));
		m_NumBones = 0;
		m_pScene = NULL;
		Clear();

		m_pScene = m_Importer.ReadFile(Filename.c_str(), ImportFlags);

		if (m_pScene) 
		{

			InitFromScene(m_pScene, Filename);
		}
		else {
			printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
		}

		// Make sure the VAO is not changed from the outside
		glBindVertexArray(0);
	}


	AnimatedMesh::~AnimatedMesh()
	{
		Clear();
	}


	void AnimatedMesh::Clear()
	{
		if (m_Buffers[0] != 0) {
			glDeleteBuffers(NUM_VBs, m_Buffers);
		}
	}



	bool AnimatedMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
	{
		m_Entries.resize(pScene->mNumMeshes);
		LoadHierarchy(m_pScene->mRootNode);
		
		std::vector<Vector3f> Positions;
		std::vector<Vector3f> Normals;
		std::vector<Vector2f> TexCoords;
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

		m_vao = std::make_shared<ArrayBuffer>();
		m_vao->Bind();

		glGenBuffers(NUM_VBs, m_Buffers);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


		return true;
	};


	void AnimatedMesh::InitMesh(unsigned int MeshIndex,
		const aiMesh* paiMesh,
		std::vector<Vector3f>& Positions,
		std::vector<Vector3f>& Normals,
		std::vector<Vector2f>& TexCoords,
		std::vector<VertexBoneData>& Bones,
		std::vector<unsigned int>& Indices
		)
	{
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		// Populate the vertex attribute vectors
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[i]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

			Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
			Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
			TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
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
				m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
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
		m_vao->Bind();
		for (unsigned int i = 0; i < m_Entries.size(); i++) {

			glDrawElementsBaseVertex(GL_TRIANGLES,
				m_Entries[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
				m_Entries[i].BaseVertex);
		}

		m_vao->UnBind();
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


	void AnimatedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
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
		Out = Start + Factor * Delta;
	}


	void AnimatedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		//assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();

	}
	

	

	void AnimatedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
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
		Out = Start + Factor * Delta;
	}

	void AnimatedMesh::LoadHierarchy(const aiNode* pNode)
	{

		m_NodeAnim[pNode->mName.data] = FindNodeAnim(m_pScene->mAnimations[0], pNode->mName.data);

		for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
			LoadHierarchy(pNode->mChildren[i]);
		}
	}

	void AnimatedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform)
	{
	
		Matrix4f NodeTransformation(pNode->mTransformation);
		if (m_NodeAnim[pNode->mName.data]) 
		{	
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, m_NodeAnim[pNode->mName.data]);
			Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());
			
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, m_NodeAnim[pNode->mName.data]);	
			Matrix4f TranslationM;
			TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);
			
			NodeTransformation = TranslationM * RotationM;
		}

		Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;
		// NEED TO PUT THOSE CALCULATIONS ON GPU
		if (m_BoneMapping.find(pNode->mName.data) != m_BoneMapping.end()) {
			unsigned int BoneIndex = m_BoneMapping[pNode->mName.data];
			
			m_FinalTransformation[BoneIndex] = GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
		}

		for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
		}
	}


	void AnimatedMesh::AnimateBones(float TimeInSeconds)
	{
		Matrix4f Identity;
		Identity.InitIdentity();

		float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

		ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);
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