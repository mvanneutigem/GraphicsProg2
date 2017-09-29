//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ModelAnimator.h"
#include "../Diagnostics/Logger.h"


ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(vector<XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}


ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber >= m_pMeshFilter->m_AnimationClips.size())
	{
		Reset();
		Logger::LogWarning(L"Clipnumber larger than amount of clips");
	}
	else
	{
		auto clip = m_pMeshFilter->m_AnimationClips[clipNumber];
		SetAnimation(clip);
	}
	//If not,
	//	Call Reset
	//	Log a warning with an appropriate message
	//	return
	//else
	//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
	//	Call SetAnimation(AnimationClip clip)
}

void ModelAnimator::SetAnimation(wstring clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	bool succes = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for(auto c : m_pMeshFilter->m_AnimationClips)
	{
		if (c.Name == clipName)
		{
			SetAnimation(c);
			succes = true;
		}
	}

	if (!succes)
	{
		Reset();
		Logger::LogWarning(L"Clipname was not found");
	}
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	
	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause)
	{
		m_IsPlaying = false;
		m_AnimationSpeed = 1.0f;
		m_TickCount = 0;
	}
		
	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f
	if (m_ClipSet)
	{
		m_Transforms.assign(m_CurrentClip.Keys[0].BoneTransforms.begin(), m_CurrentClip.Keys[0].BoneTransforms.end());
	}
	else
	{
		auto mat = XMMatrixIdentity();
		XMFLOAT4X4 xmat;
		XMStoreFloat4x4(&xmat,mat);
		m_Transforms.assign(m_pMeshFilter->m_BoneCount,xmat);
	}
	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = gameContext.pGameTime->GetElapsed() * m_CurrentClip.TicksPerSecond * m_AnimationSpeed;
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		Clamp(passedTicks, m_CurrentClip.Duration, 0.0f);
		//2. 
		//IF m_Reversed is true
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if(m_TickCount < 0)
			{
				m_TickCount += m_CurrentClip.Duration;
			}
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.Duration)
			{
				m_TickCount -= m_CurrentClip.Duration;
			}
		}
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		
		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		for(size_t i = 0; i < m_CurrentClip.Keys.size(); ++i)
		{

			if (m_CurrentClip.Keys[i].Tick > m_TickCount)
			{
				keyA = m_CurrentClip.Keys[i -1];
				keyB = m_CurrentClip.Keys[i ];
				break;
			}
		}
		
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		auto blendFactor = (m_TickCount - keyA.Tick)/ (keyB.Tick - keyA.Tick);
		//Clear the m_Transforms vector
		if (keyA.BoneTransforms.size() > 0)
		{
			m_Transforms.clear();
			//FOR every boneTransform in a key (So for every bone)

			for (auto i = 0; i< m_pMeshFilter->m_BoneCount; ++i)
			{
				auto transformA = keyA.BoneTransforms[i];
				auto transformB = keyB.BoneTransforms[i];
				XMMATRIX transformAMat = XMLoadFloat4x4(&transformA);;
				XMMATRIX transformBMat = XMLoadFloat4x4(&transformB);;

				XMVECTOR scaleA, rotA, transA;
				XMMatrixDecompose(&scaleA, &rotA, &transA, transformAMat);
				XMVECTOR scaleB, rotB, transB;
				XMMatrixDecompose(&scaleB, &rotB, &transB, transformBMat);

				auto scale = XMVectorLerp(scaleA, scaleB, blendFactor);
				auto rot = XMQuaternionSlerp(rotA, rotB, blendFactor);
				auto trans = XMVectorLerp(transA, transB, blendFactor);

				XMMATRIX transMat = XMMatrixTranslationFromVector(trans);
				XMMATRIX scaleMat = XMMatrixScalingFromVector(scale);
				XMMATRIX rotMat = XMMatrixRotationQuaternion(rot);

				auto mat = scaleMat * rotMat * transMat;
				XMFLOAT4X4 finalmat;
				XMStoreFloat4x4(&finalmat, mat);
				m_Transforms.push_back(finalmat);

			}
		}
		
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
	}
}
