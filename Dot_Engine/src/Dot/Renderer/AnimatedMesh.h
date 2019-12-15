#pragma once
#include "Dot/Renderer/Buffers/ArrayBuffer.h"

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>       
#include <assimp/postprocess.h>

#include <map>

namespace Dot {
	class AnimatedMesh
	{
	public:
		struct BoneInfo
		{
			glm::mat4 BoneOffset;

		};
	public:
		AnimatedMesh(const std::string& Filename);
		~AnimatedMesh();

		void Render();
		void AnimateBones(float TimeInSeconds);

		glm::mat4 GetInverse() { return m_InverseTransform; }
		const float GetDuration() const { return (float)m_pScene->mAnimations[0]->mDuration; }
		unsigned int GetNumBones() const { return m_NumBones; }
		const std::vector<BoneInfo>& GetBonesInfo() { return m_BoneInfo; }
		const std::vector<glm::mat4>& GetBoneTransformations() { return m_FinalTransformation; }

	private:
		struct VertexBoneData
		{
			#define NUM_BONES_PER_VERTEX 4

			VertexBoneData()
			{
				Reset();
			};

			void Reset()
			{
				memset(IDs, 0, sizeof(IDs));
				memset(Weights, 0, sizeof(Weights));
			}

			void AddBoneData(unsigned int BoneID, float Weight);

			unsigned int IDs[NUM_BONES_PER_VERTEX];
			float Weights[NUM_BONES_PER_VERTEX];

		};

	private:
		glm::vec3 CalcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::quat CalcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::vec3 CalcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		
		unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
		
		void LoadHierarchy(const aiNode* pNode,std::vector<std::string>& boneMappingDelete);
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
		
		bool InitFromScene(const aiScene* pScene, const std::string& Filename);
		void InitMesh(
			unsigned int MeshIndex,
			const aiMesh* paiMesh,
			std::vector<glm::vec3>& Positions,
			std::vector<glm::vec3>& Normals,
			std::vector<glm::vec2>& TexCoords,
			std::vector<VertexBoneData>& Bones,
			std::vector<unsigned int>& Indices
		);

		void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
	private:	
		Ref<ArrayBuffer> m_VAO;

		struct MeshEntry 
		{
			MeshEntry()
			{
				NumIndices = 0;
				BaseVertex = 0;
				BaseIndex = 0;
			}
			unsigned int NumIndices;
			unsigned int BaseVertex;
			unsigned int BaseIndex;

		};

		std::vector<BoneInfo> m_BoneInfo;
		std::vector<glm::mat4> m_FinalTransformation;
		std::vector<MeshEntry> m_Entries;


		std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index
		std::map<std::string, const aiNodeAnim*> m_NodeAnim;

		unsigned int m_NumBones;
		glm::mat4 m_InverseTransform;

		const aiScene* m_pScene;
		Assimp::Importer m_Importer;
	
		float m_Time = 0.0f;
	};



}

