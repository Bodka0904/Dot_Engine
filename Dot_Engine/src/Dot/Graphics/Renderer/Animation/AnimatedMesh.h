#pragma once
#include "AssimpMat.h"
#include "Dot/Graphics/Renderer/ArrayBuffer.h"

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
			Matrix4f BoneOffset;

			BoneInfo()
			{
				BoneOffset;
			}
		};
	public:
		AnimatedMesh(const std::string& Filename);
		~AnimatedMesh();

		void Render();
		void AnimateBones(float TimeInSeconds);

		unsigned int GetNumBones() const { return m_NumBones; }
		const std::vector<BoneInfo>& GetBonesInfo() { return m_BoneInfo; }
		const std::vector<Matrix4f>& GetBoneTransformations() { return m_FinalTransformation; }

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
		void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		
		unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
		
		void LoadHierarchy(const aiNode* pNode);
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);
		
		bool InitFromScene(const aiScene* pScene, const std::string& Filename);
		void InitMesh(
			unsigned int MeshIndex,
			const aiMesh* paiMesh,
			std::vector<Vector3f>& Positions,
			std::vector<Vector3f>& Normals,
			std::vector<Vector2f>& TexCoords,
			std::vector<VertexBoneData>& Bones,
			std::vector<unsigned int>& Indices
		);

		void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
		void Clear();

	private:	
		enum VB_TYPES {
			INDEX_BUFFER,
			POS_VB,
			NORMAL_VB,
			TEXCOORD_VB,
			BONE_VB,
			NUM_VBs
		};

		Ref<ArrayBuffer>m_vao;
		unsigned int m_Buffers[NUM_VBs];

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
		std::vector<Matrix4f> m_FinalTransformation;
		std::vector<MeshEntry> m_Entries;


		std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index
		std::map<std::string, const aiNodeAnim*> m_NodeAnim;

		unsigned int m_NumBones;
	
		const aiScene* m_pScene;
		Assimp::Importer m_Importer;
	
	};



}

