#include "stdafx.h"
#include "AnimationSystem.h"

#include <assert.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
	void AnimationSystem::Update(float ts)
	{
		for (int i = 0; i < m_Entities.size(); ++i)
		{
			m_Current = &ECSManager::Get()->GetComponent<AnimationComponent>(m_Entities[i]);
			if (m_Current->isAnimating)
				m_Current->animation["walk"]->time += ts;
			else
				m_Current->animation["walk"]->time = 0.0f;

			float TicksPerSecond = (float)(m_Current->animation["walk"]->pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_Current->animation["walk"]->pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
			float TimeInTicks = m_Current->animation["walk"]->time * TicksPerSecond;
			float AnimationTime = fmod(TimeInTicks, (float)m_Current->animation["walk"]->pScene->mAnimations[0]->mDuration);
			if (AnimationTime >= m_Current->animation["walk"]->pScene->mAnimations[0]->mDuration)
				m_Current->animation["walk"]->time = 0;
			
			readNodeHeirarchy(AnimationTime, m_Current->animation["walk"]->pScene->mRootNode, glm::mat4(1.0f));
		}
	}
	void AnimationSystem::Add(Entity entity)
	{
		LOG_INFO("Entity with ID %d",entity);
		m_Entities.push_back(entity);
	}
	void AnimationSystem::Remove(Entity entity)
	{
		if (!m_Entities.empty())
		{
			std::sort(m_Entities.begin(), m_Entities.end(), m_Cmp);
			// get the range in 2*log2(N), N=vec.size()
			auto bounds = std::equal_range(m_Entities.begin(), m_Entities.end(), entity);
			// calculate the index of the first to be deleted O(1)
			auto last = m_Entities.end() - std::distance(bounds.first, bounds.second);
			// swap the 2 ranges O(equals) , equal = std::distance(bounds.first, bounds.last)
			std::swap_ranges(bounds.first, bounds.second, last);
			// erase the victims O(equals)
			m_Entities.erase(last, m_Entities.end());
		}
	}
	void AnimationSystem::EntityUpdated(Entity entity)
	{
	}
	void AnimationSystem::readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
	{
		glm::mat4 NodeTransformation(aiMatrix4x4ToGlm(pNode->mTransformation));
		glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

		if (m_Current->animation["walk"]->nodeAnim[pNode->mName.data])
		{
			glm::quat RotationQ = calcInterpolatedRotation(AnimationTime, m_Current->animation["walk"]->nodeAnim[pNode->mName.data]);
			glm::mat4 RotationM = glm::toMat4(RotationQ);

			glm::vec3 Translation = calcInterpolatedPosition(AnimationTime, m_Current->animation["walk"]->nodeAnim[pNode->mName.data]);
			glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), glm::vec3(Translation.x, Translation.y, Translation.z));;


			NodeTransformation = TranslationM * RotationM;

			GlobalTransformation = ParentTransform * NodeTransformation;

			unsigned int BoneIndex = m_Current->animation["walk"]->boneMapping[pNode->mName.data];
			m_Current->animation["walk"]->boneInfo[BoneIndex].FinalTransformation = GlobalTransformation * m_Current->animation["walk"]->boneInfo[BoneIndex].BoneOffset;

		}

		for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		{
			readNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
		}
	}
	glm::vec3 AnimationSystem::calcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
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
	glm::quat AnimationSystem::calcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
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
	glm::vec3 AnimationSystem::calcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
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
	unsigned int AnimationSystem::findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
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
	unsigned int AnimationSystem::findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
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
	unsigned int AnimationSystem::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
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
}